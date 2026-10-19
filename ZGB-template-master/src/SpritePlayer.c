#include "Banks/SetAutoBank.h"

#include "Keys.h"
#include "SpriteManager.h"

INT8 y_velocity = 0;
UINT8 gravity_counter = 0;
UINT8 jump_counter = 0;
UINT8 on_ground = 0;

#define GRAVITY_DELAY 3
#define JUMP_GRAVITY_DELAY 4
#define JUMP_STRENGTH -4
#define MAX_FALL_SPEED 2

const UINT8 anim_idle[] = {2, 0, 1};
const UINT8 anim_walk[] = {4, 2, 3, 4, 5};
const UINT8 anim_blow[] = {2, 6, 7};
const UINT8 anim_blow_up[] = {2, 1, 8};
const UINT8 anim_crouch[] = {1, 1};

void START() {
}

void UPDATE() {
    if(KEY_PRESSED(J_LEFT) && !KEY_PRESSED(J_DOWN) && !KEY_PRESSED(J_B)){
        TranslateSprite(THIS, -1, 0);
        SetSpriteAnim(THIS, anim_walk, 10);
        THIS->mirror = V_MIRROR;
    }
    if(KEY_PRESSED(J_RIGHT) && !KEY_PRESSED(J_DOWN) && !KEY_PRESSED(J_B)){
        TranslateSprite(THIS, 1, 0);
        SetSpriteAnim(THIS, anim_walk, 10);
        THIS->mirror = NO_MIRROR;
    }
    if(KEY_PRESSED(J_B)){
        if(KEY_PRESSED(J_DOWN)){
            SetSpriteAnim(THIS, anim_blow, 25);
        }
    }
    if(KEY_PRESSED(J_B)){
        if(!KEY_PRESSED(J_DOWN)){
            SetSpriteAnim(THIS, anim_blow_up, 25);
        }
    }
    if(KEY_PRESSED(J_DOWN) && ! KEY_PRESSED(J_B)){
        SetSpriteAnim(THIS, anim_crouch, 0);
    }

    // Jump
    if(KEY_PRESSED(J_A) && on_ground && !KEY_PRESSED(J_B)) {
        y_velocity = JUMP_STRENGTH;
        on_ground = 0;
        gravity_counter = 0;
        jump_counter = 0;
    }

    // Apply gravity more slowly while rising
    if(y_velocity < 0) {
        if(++jump_counter >= JUMP_GRAVITY_DELAY) {
            jump_counter = 0;
            y_velocity++;
        }
    } else {
        // Normal gravity while falling
        if(++gravity_counter >= GRAVITY_DELAY) {
            gravity_counter = 0;

            if(y_velocity < MAX_FALL_SPEED) {
                y_velocity++;
            }
        }
    }

    // Move and check collisions
    UINT8 hit = TranslateSprite(THIS, 0, y_velocity);

    if(hit != 0) {
        if(y_velocity > 0) {
            y_velocity = 0;
            on_ground = 1;
        } else if(y_velocity < 0) {
            y_velocity = 0;
        }
    } else {
        on_ground = 0;
    }

    if(keys == 0){
        SetSpriteAnim(THIS, anim_idle, 5);
    }
}

void DESTROY() {
}