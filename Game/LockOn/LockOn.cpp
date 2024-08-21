#include "LockOn.h"
#include "Input.h"
#include "TextureManager.h"
#include <cmath>
#include <cstdlib>
void LockOn::Init()
{
	lockOnMark_ = std::make_unique<Sprite>();
	lockOnMark_->Init(
		{ 720.0f ,360.0f },
		{ 64.0f, 64.0f },
		{ 0.5f , 0.5f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		"Resources/Reticle.png");
	lockOnMarkTex_ = TextureManager::StoreTexture("Resources/Reticle.png");
}

void LockOn::Update(const std::list<Enemy*>& enemies, Camera* camera)
{
	if (!target_) {
		// ロックオンボタンをトリガーしたら
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_Y)) {
			// ロックオン対象の検索
			// 目標
			std::list<std::pair<float, const Enemy*>> targets;

			// 全ての敵に対して順にロックオン判定
			for (Enemy* enemy : enemies) {
				// 敵のロックオン座標取得
				Vector3 positionWorld = enemy->GetWorldPosition();

				// ワールド→ビュー座標返還
				Vector3 positionView = Transform1(positionWorld, camera->GetViewMatrix());
				// 距離条件チェック
				if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
					// カメラ前方との角度を計算
					float arcTangent = std::atan2(
						std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y),
						positionView.z);

					// 角度条件チェック（コーンにおさまっているか）
					if (std::abs(arcTangent) <= angleRange_) {
						targets.emplace_back(std::make_pair(positionView.z, enemy));

					}
				}

				// ロックオン対象をリセット
				target_ = nullptr;
				if (targets.size() != 0) {
					// 距離が昇順にソート
					targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
					// ソートの結果一番近い敵をロックオン対象とする
					target_ = targets.front().second;
				}

			}
		}
	}
	// ロックオン継続
	else if (target_) {
		
		// 敵のロックオン座標取得
		Vector3 positionWolrd = target_->GetWorldPosition();
		// ワールド座標からスクリーン座標に変換
			// ビューポート行列
		Matrix4x4 matView = MakeViewportMatrix(0, 0, WinAPI::kClientWidth_, WinAPI::kClientHeight_, 0, 1);

		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport =
			Multiply(camera->GetViewprojectionMatrix(), matView);

		// ワールド→スクリーン座標変換(ここで3Dから2dになる)
		Vector3 pos = Transform1(positionWolrd, matViewProjectionViewport);

		lockOnMark_->SetPosition({ pos.x,pos.y });
		if (Input::GetInstance()->TriggerJoyButton(XINPUT_GAMEPAD_Y)) {
			target_ = nullptr;
		}
		else if (OutRangeJudge(camera)) {
			target_ = nullptr;
		}
	}
	lockOnMark_->Update();
}

void LockOn::Draw()
{
	if (target_ != nullptr) {
		lockOnMark_->Draw(lockOnMarkTex_,{1.0f,1.0f,1.0f,1.0f});
	}
}

bool LockOn::OutRangeJudge(Camera* camera)
{
	// 敵のロックオン座標取得
	Vector3 positionWolrd = target_->GetWorldPosition();

	// ワールド→ビュー座標返還
	Vector3 positionView = Transform1(positionWolrd, camera->GetViewMatrix());
	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		// カメラ前方との角度を計算
		float arcTangent = std::atan2(
			std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y),
			positionView.z);

		// 角度条件チェック（コーンにおさまっているか）
		if (std::abs(arcTangent) <= angleRange_) {
			return false;

		}
	}
	return true;
}
