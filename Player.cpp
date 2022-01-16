#include "Player.h"

Player::Player() {
	player = {
		{
			100,
			800
		},
		20,
		0,
		0
	};

	oldPlayer = {
		0,
		0
	};

	hp = 3;
	isDamage = 0;
	isDamageTimer = 0;

	inertia = 0;
	inertiaSpeed = 0;

	isButton = 0;

	//左上の座標
	leftTopX = 0;
	leftTopY = 0;
	//左下の座標
	leftBottomX = 0;
	leftBottomY = 0;
	//右下の座標
	rightTopX = 0;
	rightTopY = 0;
	//右下の座標
	rightBottomX = 0;
	rightBottomY = 0;
	//1フレーム前の左上の座標
	oldLeftTopX = 0;
	oldLeftTopY = 0;
	//1フレーム前の右上の座標
	oldRightTopX = 0;
	oldRightTopY = 0;
	//1フレーム前の左下の座標
	oldLeftBottomX = 0;
	oldLeftBottomY = 0;
	//1フレーム前の右下の座標
	oldRightBottomX = 0;
	oldRightBottomY = 0;

	scroll = 0;

	bullet = new Bullet;
	map = new Map;
}

Player::~Player() {
	delete bullet;
	delete map;
}

void Player::SaveOldPlayer() {
	oldPlayer.x = player.transform.x;
	oldPlayer.y = player.transform.y;
}

void Player::GetPlayerBottom(int BLOCK_SIZE) {
	//左下の座標の取得
	leftBottomX = (player.transform.x - player.r) / BLOCK_SIZE;
	leftBottomY = (player.transform.y + player.r - 1 + 1) / BLOCK_SIZE;
	//右下の座標の取得
	rightBottomX = (player.transform.x + player.r - 1) / BLOCK_SIZE;
	rightBottomY = (player.transform.y + player.r - 1 + 1) / BLOCK_SIZE;
}

void Player::ResetIsJump(int map[][50]) {
	player.isJump = true;
	if (map[leftBottomY][leftBottomX] == BLOCK || map[rightBottomY][rightBottomX] == BLOCK) {
		player.isJump = false;
	}
}

void Player::PlayerMove(int LInputX, int RInputX, int RInputY, int isRescued) {
	if (LInputX > 0 || LInputX < 0) {
		player.transform.x += LInputX / 200;
	}
	player.transform.y += G - player.jumpPow;

	inertia = 0;
	if (isRescued == false) {
		if ((RInputX <= 0 && RInputY < 0) || (RInputX <= 0 && RInputY > 0) || RInputX < 0) {
			player.transform.x += (RInputX * -1) / 180;
			inertia = (RInputY * -1) / 40;
		}
	}

	if (inertia > inertiaSpeed) {
		inertiaSpeed++;
	}
	else if (inertia < inertiaSpeed) {
		inertiaSpeed--;
	}
	player.transform.y += inertiaSpeed;
}

void Player::PlayerJump(int pad, int isRescued) {
	if (pad & PAD_INPUT_5) {
		if (isButton == 0 && player.isJump == 0) {
			player.isJump = 1;
			isButton = 1;
			if (isRescued == false) {
				player.jumpPow = 30;
			}
			else {
				player.jumpPow = 25;
			}
		}
	}
	else {
		isButton = 0;
	}

	if (player.isJump == true && player.jumpPow > 0) {
		player.jumpPow--;
	}
}

void Player::PlayerShot(int InputX, int InputY, int isRescued) {
	if (isRescued == false) {
		if ((InputX <= 0 && InputY < 0) || (InputX <= 0 && InputY > 0) || InputX < 0) {
			bullet->BulletShot(player.transform, InputX, InputY);
		}
	}
}

void Player::PlayerDamage(int fireX, int fireY, int fireR ,int isFire) {
	if (isDamage == 0 && isFire == true) {
		if (fireX - fireR < player.transform.x + player.r &&
			player.transform.x - player.r < fireX + fireR &&
			fireY - fireR < player.transform.y + player.r &&
			player.transform.y - player.r < fireY + fireR) {

			hp--;
			isDamage = 1;
			isDamageTimer = 100;
		}
	}
}

void Player::DamageCount() {
	if (isDamage == 1) {
		isDamageTimer--;
		if (isDamageTimer <= 0) {
			isDamage = 0;
		}
	}
}

void Player::GetPlayer(int BLOCK_SIZE) {
	//左上の座標の取得
	leftTopX = (player.transform.x - player.r) / BLOCK_SIZE;
	leftTopY = (player.transform.y - player.r) / BLOCK_SIZE;
	//右上の座標の取得
	rightTopX = (player.transform.x + player.r - 1) / BLOCK_SIZE;
	rightTopY = (player.transform.y - player.r) / BLOCK_SIZE;
	//左下の座標の取得
	leftBottomX = (player.transform.x - player.r) / BLOCK_SIZE;
	leftBottomY = (player.transform.y + player.r - 1) / BLOCK_SIZE;
	//右下の座標の取得
	rightBottomX = (player.transform.x + player.r - 1) / BLOCK_SIZE;
	rightBottomY = (player.transform.y + player.r - 1) / BLOCK_SIZE;

	//ジャンプ用の座標を取得
	jumpLeftBottomX = (player.transform.x - player.r) / BLOCK_SIZE;
	jumpLeftBottomY = (player.transform.y + player.r - 1 - G) / BLOCK_SIZE;
	jumpRightBottomX = (player.transform.x + player.r - 1) / BLOCK_SIZE;
	jumpRightBottomY = (player.transform.y + player.r - 1 - G) / BLOCK_SIZE;
}

void Player::GetOldPlayer(int BLOCK_SIZE) {
	//1フレーム前の左上の座標の取得
	oldLeftTopX = (oldPlayer.x - player.r) / BLOCK_SIZE;
	oldLeftTopY = (oldPlayer.y - player.r) / BLOCK_SIZE;
	//1フレーム前の右上の座標の取得
	oldRightTopX = (oldPlayer.x + player.r - 1) / BLOCK_SIZE;
	oldRightTopY = (oldPlayer.y - player.r) / BLOCK_SIZE;
	//1フレーム前の左下の座標の取得
	oldLeftBottomX = (oldPlayer.x - player.r) / BLOCK_SIZE;
	oldLeftBottomY = (oldPlayer.y + player.r - 1) / BLOCK_SIZE;
	//1フレーム前の右下の座標の取得
	oldRightBottomX = (oldPlayer.x + player.r - 1) / BLOCK_SIZE;
	oldRightBottomY = (oldPlayer.y + player.r - 1) / BLOCK_SIZE;
}

void Player::GetScroll() {
	if (player.transform.x >= WIN_WIDTH / 2 && player.transform.x <= WIN_WIDTH + (48 * 10)) {
		scroll = player.transform.x - WIN_WIDTH / 2;
	}
}

void Player::BlockCollision(int map[][50]) {
	if ((map[rightTopY][rightTopX] == BLOCK && map[leftTopY][leftTopX] == BLOCK) && map[leftBottomY][leftBottomX] != BLOCK && map[rightBottomY][rightBottomX] != BLOCK) {
		player.jumpPow = 0;
	}
	if (map[leftBottomY][leftBottomX] == BLOCK) {
		if (player.jumpPow <= 0) {
			player.isJump = false;
		}
		if (map[oldLeftBottomY][leftBottomX] != BLOCK && map[leftBottomY][oldLeftBottomX] != BLOCK) {}

		else if (map[oldLeftBottomY][leftBottomX] != BLOCK && map[leftBottomY][oldLeftBottomX] == BLOCK) {
			player.transform.y = oldPlayer.y;
		}

		else if (map[oldLeftBottomY][leftBottomX] == BLOCK && map[leftBottomY][oldLeftBottomX] != BLOCK) {
			player.transform.x = oldPlayer.x;
		}

		else if (map[oldLeftBottomY][leftBottomX] == BLOCK && map[leftBottomY][oldLeftBottomX] == BLOCK) {
			player.transform.x = oldPlayer.x;
			player.transform.y = oldPlayer.y;
		}
	}
	if (map[rightBottomY][rightBottomX] == BLOCK) {
		if (player.jumpPow <= 0) {
			player.isJump = false;
		}
		if (map[oldRightBottomY][rightBottomX] != BLOCK && map[rightBottomY][oldRightBottomX] != BLOCK) {}

		else if (map[oldRightBottomY][rightBottomX] != BLOCK && map[rightBottomY][oldRightBottomX] == BLOCK) {
			player.transform.y = oldPlayer.y;
		}

		else if (map[oldRightBottomY][rightBottomX] == BLOCK && map[rightBottomY][oldRightBottomX] != BLOCK) {
			player.transform.x = oldPlayer.x;
		}

		else if (map[oldRightBottomY][rightBottomX] == BLOCK && map[rightBottomY][oldRightBottomX] == BLOCK) {
			player.transform.x = oldPlayer.x;
			player.transform.y = oldPlayer.y;
		}
	}
	if (map[leftTopY][leftTopX] == BLOCK) {

		if (map[oldLeftTopY][leftTopX] == NONE && map[leftTopY][oldLeftTopX] == NONE) {}

		else if (map[oldLeftTopY][leftTopX] == NONE && map[leftTopY][oldLeftTopX] == BLOCK) {
			player.transform.y = oldPlayer.y;

		}

		else if (map[oldLeftTopY][leftTopX] == BLOCK && map[leftTopY][oldLeftTopX] == NONE) {
			player.transform.x = oldPlayer.x;
		}

		else if (map[oldLeftTopY][leftTopX] == BLOCK && map[leftTopY][oldLeftTopX] == BLOCK) {
			player.transform.x = oldPlayer.x;
			player.transform.y = oldPlayer.y;
		}
	}
	if (map[rightTopY][rightTopX] == BLOCK) {

		if (map[oldRightTopY][rightTopX] == NONE && map[rightTopY][oldRightTopX] == NONE) {}

		else if (map[oldRightTopY][rightTopX] == NONE && map[rightTopY][oldRightTopX] == BLOCK) {
			player.transform.y = oldPlayer.y;

		}

		else if (map[oldRightTopY][rightTopX] == BLOCK && map[rightTopY][oldRightTopX] == NONE) {
			player.transform.x = oldPlayer.x;
		}

		else if (map[oldRightTopY][rightTopX] == BLOCK && map[rightTopY][oldRightTopX] == BLOCK) {
			player.transform.x = oldPlayer.x;
			player.transform.y = oldPlayer.y;
		}
	}

	if (map[jumpLeftBottomY][jumpLeftBottomX] == BLOCK || map[jumpRightBottomY][jumpRightBottomX] == BLOCK) {
		player.isJump = true;
	}
}

void Player::DownPlayer(int map[][50], int BLOCK_SIZE) {
	if (player.isJump == false && player.jumpPow <= G) {
		for (int i = 0; i < 100; i++) {
			if (map[leftBottomY][leftBottomX] == BLOCK || map[rightBottomY][rightBottomX] == BLOCK) {
				break;
			}
			else {
				player.transform.y++;
				GetPlayerBottom(BLOCK_SIZE);
			}
		}
	}
}

void Player::DrawPlayer() {
	if (isDamageTimer % 5 != 1 && isDamageTimer % 5 != 2) {
		DrawBox(player.transform.x - player.r - scroll, player.transform.y - player.r,
			player.transform.x + player.r - scroll, player.transform.y + player.r, GetColor(200, 200, 200), true);
	}
}

void Player::DrawHp() {
	for (int i = 0; i < 3; i++) {
		DrawCircle(player.transform.x - scroll, player.transform.y - 10 + (i * 10), 4, GetColor(55, 55, 55), false);
	}

	for (int i = 0; i < hp; i++) {
		DrawCircle(player.transform.x - scroll, player.transform.y - 10 + (i * 10), 4, GetColor(55, 55, 55), true);
	}
}