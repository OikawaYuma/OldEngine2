#include "CollisionManager.h"
#include "GameScene.h"
#include "Enemy/Enemy.h"
#include "Player/Player.h"
#include "Item/Item.h"
//#include "AxisIndicator.h"

void CollisionManager::CheckAllCollision() {

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->Getbullet();

	 //敵弾リストの取得
	const std::list<Enemy*>& enemy = gameScene_->Getbullet();

	const std::list<Item*>& items = gameScene_->GetItems();

	const std::list<EnemyBullet*>& enemyBullets = gameScene_->GetEnemyBullets();

	std::list<Collider*> colliders_;
	// コライダーをリストに登録
	colliders_.push_back(player_);
	//colliders_.push_back(enemy_);

	for (PlayerBullet* bullet : playerBullets) {
		colliders_.push_back(bullet);
	}
	for (Enemy* bullet : enemy) {
		colliders_.push_back(bullet);
	}
	for (Item* bullet : items) {
		colliders_.push_back(bullet);
	}
	for (EnemyBullet* bullet : enemyBullets) {
		colliders_.push_back(bullet);
	}

	// std::list<Collider*> colliders;
	//  リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {

		// イテレータBはイテレータ―Aの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {

			// ペアの当たり判定
			CheckCollisionPair(*itrA, *itrB);
		}
	}
}

void CollisionManager::PushClider(Collider* collider)
{
	//colliders_.push_back(collider);
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	if ((colliderA->GetCollisonAttribute() & colliderB->GetCollisionMask()) ==
		(colliderB->GetCollisonAttribute() & colliderA->GetCollisionMask())) {
		return;
	};


	if (colliderA->GetCollisionMode() == Ballc && colliderB->GetCollisionMode() == Ballc) {
		// 判定対象AとBの座標
		Vector3 posA, posB;
		float radiusA, radiusB;

		// colliderAの座標
		posA = colliderA->GetWorldPosition();
		radiusA = colliderA->GetRadius();

		// colliderBの座標
		posB = colliderB->GetWorldPosition();
		radiusB = colliderB->GetRadius();
		// 弾と弾の考交差判定
		// 衝突フィルタリング

		float p2b = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z);
		float r2r = (radiusA + radiusB) * (radiusA + radiusB);
		/*  if (((colliderA->GetCollisonAttribute() & colliderB->GetCollisionMask())!=0) ||
			  ((colliderB->GetCollisonAttribute() & colliderA->GetCollisionMask()))!=0) {
		  return;
		  };*/

		if (p2b <= r2r) {

			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision(colliderB->GetCollisonAttribute());
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision(colliderA->GetCollisonAttribute());
		}
	}
	if (
		(colliderA->GetCollisionMode() == Ballc && colliderB->GetCollisionMode() == OBBc) ||
		(colliderA->GetCollisionMode() == OBBc && colliderB->GetCollisionMode() == Ballc)
		) {
		// 判定対象AとBの座標
		Vector3 posA, posB;
		float radiusA, radiusB;

		// colliderAの座標
		posA = colliderA->GetWorldPosition();
		radiusA = colliderA->GetRadius();

		// colliderBの座標
		posB = colliderB->GetWorldPosition();
		radiusB = colliderB->GetRadius();
		// 弾と弾の考交差判定
		// 衝突フィルタリング

		float p2b = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z);
		float r2r = (radiusA + radiusB) * (radiusA + radiusB);
		/*  if (((colliderA->GetCollisonAttribute() & colliderB->GetCollisionMask())!=0) ||
			  ((colliderB->GetCollisonAttribute() & colliderA->GetCollisionMask()))!=0) {
		  return;
		  };*/

		//if (IsCollisionAABB()) {

		//	// コライダーAの衝突時コールバックを呼び出す
		//	colliderA->OnCollision(colliderB->GetCollisonAttribute());
		//	// コライダーBの衝突時コールバックを呼び出す
		//	colliderB->OnCollision(colliderA->GetCollisonAttribute());
		//}
	}
};
