#include "Collision.h"
#include <DirectXMath.h>
#include <cmath>
//玉と玉の交差判定
bool Collision::IntersectSphereVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outpositionB)
{
    //A->Bの単位ベクトルを算出
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    //距離判定
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false;
    }
    
    //AがBを押し出す
    Vec = DirectX::XMVector3Normalize(Vec);
    Vec = DirectX::XMVectorScale(Vec, range);
    PositionB = DirectX::XMVectorAdd(PositionA, Vec);

    // 新しい位置をfloat配列に変換して返す
    DirectX::XMStoreFloat3(&outpositionB, PositionB);

    return true;
}

//円柱と円柱の交差判定
bool Collision::IntersectCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outpositionB)
{
    //Aの足元がBの頭より上なら当たっていない
    if (positionA.y > positionB.y + heightA)
    {
        return false;
    }
    
    //Aの頭がBの足元より下なら当たっていない
    if (positionA.y + heightA < positionB.y)
    {
        return false;
    }

    //XZ平面での範囲チェック
    float vx = positionB.x - positionA.x;
    float vz = positionB.z - positionA.z;
    float range = radiusA + radiusB;
    float distXZ = sqrtf(vx * vx + vz * vz);
    if (distXZ > range)
    {
        return false;
    }


    //AがBを押し出す
    vx /= distXZ;
    vz /= distXZ;

    outpositionB.x = positionA.x + (vx * range);
    outpositionB.y = positionB.y;
    outpositionB.z = positionA.z + (vz * range);

    return true;
}

bool Collision::IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight, DirectX::XMFLOAT3& outCylinderPosition)
{
   /* if (spherePosition.y + sphereRadius < cylinderPosition.y)return false;
    if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)return false;

    float vx = cylinderPosition.x - spherePosition.x;
    float vz = cylinderPosition.z - spherePosition.z;
    float range = sphereRadius + cylinderRadius;
    float distXZ = sqrtf(vx * vx + vz * vz);
    if (distXZ > range)return false;

    vx /= distXZ;
    vz /= distXZ;
    outCylinderPosition.x = spherePosition.x + (vx * range);
    outCylinderPosition.y = spherePosition.y;
    outCylinderPosition.z = spherePosition.z + (vz * range);*/

    // Calculate the squared distance between the sphere's center and the cylinder's axis
    DirectX::XMFLOAT3 diff = DirectX::XMFLOAT3(spherePosition.x - cylinderPosition.x, 0, spherePosition.z - cylinderPosition.z);
    float distSquaredXZ = diff.x * diff.x + diff.z * diff.z;
    float radiusSum = sphereRadius + cylinderRadius;

    // Check if the sphere is within the infinite cylinder defined by the cylinder's radius
    if (distSquaredXZ > radiusSum * radiusSum)
    {
        return false;
    }

    // Check the height boundaries of the cylinder
    float cylinderTop = cylinderPosition.y + cylinderHeight / 2.0f;
    float cylinderBottom = cylinderPosition.y - cylinderHeight / 2.0f;
    if (spherePosition.y + sphereRadius < cylinderBottom || spherePosition.y - sphereRadius > cylinderTop)
    {
        return false;
    }

    // Calculate the closest point on the cylinder's axis to the sphere
    float clampedY = spherePosition.y;
    if (spherePosition.y < cylinderBottom)
    {
        clampedY = cylinderBottom;
    }
    else if (spherePosition.y > cylinderTop)
    {
        clampedY = cylinderTop;
    }
    float distY = abs(spherePosition.y - clampedY);

    

    // Check if the sphere is intersecting with the top or bottom cap of the cylinder
    if (distSquaredXZ <= cylinderRadius * cylinderRadius && distY <= sphereRadius)
    {
        // Calculate collision point on the top or bottom cap
        outCylinderPosition = DirectX::XMFLOAT3(spherePosition.x, clampedY, spherePosition.z);
        return true;
    }

    // Check if the sphere is intersecting with the side surface of the cylinder
    if (distSquaredXZ <= radiusSum * radiusSum)
    {
        float distXZ = sqrtf(distSquaredXZ);
        float penetrationDepth = radiusSum - distXZ;

        // Normalize the xz distance
        DirectX::XMFLOAT3 penetrationVector = DirectX::XMFLOAT3(diff.x / distXZ * penetrationDepth, 0, diff.z / distXZ * penetrationDepth);
        outCylinderPosition = DirectX::XMFLOAT3(spherePosition.x - penetrationVector.x, spherePosition.y, spherePosition.z - penetrationVector.z);
        return true;
    }

    return false;
}

bool Collision::IntersectRayVsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
    const Model* model, 
    HitResult& result)
{
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    //メッシュノード取得
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    //レイをワールド空間からローカル空間へ変換
    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        //レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        //三角形（面）との交差判定
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        const std::vector<UINT> indices = mesh.indices;

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;

                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
                float dot;
                DirectX::XMStoreFloat(&dot, Dot);
                if (dot >= 0.0f)
                {
                    continue;
                }

                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(SA, N), Dot);
                float x;
                DirectX::XMStoreFloat(&x, X);
                if (x < .0f || x > neart) continue;


                DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));

                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(N, Cross1);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)
                {
                    continue;
                }

                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB,BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(N, Cross2);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)
                {
                    continue;
                }

                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC,CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(N, Cross3);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)
                {
                    continue;
                }

                neart = x;

                HitPosition = P;
                HitNormal = N;
                materialIndex = subset.materialIndex;

            }
        }  
        if (materialIndex >= 0)
        {
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            //ヒット情報保存
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

                result.distance = distance;
                result.materialIndex = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }

    return hit;
}
