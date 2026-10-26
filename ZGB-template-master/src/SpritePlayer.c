//TODO
//Projectiles

#include "Banks/SetAutoBank.h"

#include "Keys.h"
#include "SpriteManager.h"
#include "stdio.h"

#define GRAVITY_DELAY 3
#define JUMP_GRAVITY_DELAY 4
#define JUMP_STRENGTH -4
#define MAX_FALL_SPEED 3
#define MAX_JUMP_HOLD 3
#define JUMP_CUT_SPEED 1

const UINT8 anim_idle[] = {2, 0, 1};
const UINT8 anim_walk[] = {4, 2, 3, 4, 5};
const UINT8 anim_blow[] = {2, 6, 7};
const UINT8 anim_blow_up[] = {2, 1, 8};
const UINT8 anim_crouch[] = {1, 1};
const UINT8 anim_jump[] = {1, 9};

INT8 y_velocity = 0;
UINT8 gravity_counter = 0;
UINT8 jump_counter = 0;
UINT8 on_ground = 0;
UINT8 jump_held_frames = 0;
UINT8 dash_frames = 0;
UINT8 dash_eligible_right = 0;
UINT8 dash_eligible_left = 0;
UINT8 run_eligible = 0;
UINT8 step_toggle = 0;

void START() {
}

void UPDATE() {
    //Double tapping one direction enables running
    if(dash_eligible_left == 1){
        if(KEY_TICKED(J_LEFT)){
            run_eligible = 1;
        }
    }
    if(dash_eligible_right == 1){
        if(KEY_TICKED(J_RIGHT)){
            run_eligible = 1;
        }
    }

    //Initial press of direction start timer during which running can occur
    if(on_ground && !KEY_PRESSED(J_B) && !KEY_PRESSED(J_DOWN)){
        if(KEY_TICKED(J_LEFT)){
            dash_eligible_left = 1;
            dash_eligible_right = 0;
            dash_frames = 0;
        }
        else if(KEY_TICKED(J_RIGHT)){
            dash_eligible_right = 1;
            dash_eligible_left = 0;
            dash_frames = 0;
        }
    }

    //Timer allowing repeated input for 16 frames
    if(dash_eligible_left || dash_eligible_right){
        if(dash_frames < 16){
            dash_frames++;
        }
        else{
            dash_eligible_left = 0;
            dash_eligible_right = 0;
        }
    }

    //Reset process if player stops moving
    if(KEY_RELEASED(J_LEFT) || KEY_RELEASED(J_RIGHT)){
        run_eligible = 0;
    }

    if(KEY_PRESSED(J_LEFT) && !KEY_PRESSED(J_DOWN) && !KEY_PRESSED(J_B)) {
        THIS->mirror = V_MIRROR;
        if(on_ground) {
            if(run_eligible == 1){
                step_toggle ^= 1;
                TranslateSprite(THIS, -(1 + step_toggle), 0);
                SetSpriteAnim(THIS, anim_walk, 15);       
            }
            else{
                TranslateSprite(THIS, -1, 0);
                SetSpriteAnim(THIS, anim_walk, 10);
            }
        }
        else {
            if(run_eligible == 1){
                step_toggle ^= 1;
                TranslateSprite(THIS, -(1 + step_toggle), 0);
                SetSpriteAnim(THIS, anim_jump, 0);
            }
            else{
                TranslateSprite(THIS, -1, 0);
                SetSpriteAnim(THIS, anim_jump, 0);
            }
        }
        
    }

    if(KEY_PRESSED(J_RIGHT) && !KEY_PRESSED(J_DOWN) && !KEY_PRESSED(J_B)) {
        THIS->mirror = NO_MIRROR;
            if(on_ground) {
                if(run_eligible == 1){
                    step_toggle ^= 1;
                    TranslateSprite(THIS, (1 + step_toggle), 0);
                    SetSpriteAnim(THIS, anim_walk, 15);
                }
                else{
                    TranslateSprite(THIS, 1, 0);
                    SetSpriteAnim(THIS, anim_walk, 10);
                }
            }
            else {
                if(run_eligible == 1){
                    step_toggle ^= 1;
                    TranslateSprite(THIS, (1 + step_toggle), 0);
                    SetSpriteAnim(THIS, anim_jump, 0);
                }
                else{
                    TranslateSprite(THIS, 1, 0);
                    SetSpriteAnim(THIS, anim_jump, 0);
                }
            }
    }

    if(KEY_PRESSED(J_B) && on_ground) {
        if(KEY_PRESSED(J_DOWN)) {
            SetSpriteAnim(THIS, anim_blow, 25);
        }
        else {
            SetSpriteAnim(THIS, anim_blow_up, 25);
        }
    }

    if(KEY_PRESSED(J_DOWN) && ! KEY_PRESSED(J_B)) {
        SetSpriteAnim(THIS, anim_crouch, 0);
    }

    // Start jump
    if(KEY_TICKED(J_A) && on_ground) {
        y_velocity = JUMP_STRENGTH;
        on_ground = 0;
        jump_held_frames = 0;
        gravity_counter = 0;
        SetSpriteAnim(THIS, anim_jump, 0);
    }

    // Variable jump height
    UINT8 holding_jump = 0;
    if(y_velocity < 0) {
        if(KEY_PRESSED(J_A)) {
            if(jump_held_frames < MAX_JUMP_HOLD) {
                jump_held_frames++;
                holding_jump = 1;   // no gravity this frame
            }
        } else if(y_velocity < -JUMP_CUT_SPEED) {
            y_velocity = -JUMP_CUT_SPEED;   // released early: cut the jump
        }
    }

    // Gravity (skipped while the jump is being held)
    if(!holding_jump) {
        if(++gravity_counter >= GRAVITY_DELAY) {
            gravity_counter = 0;
            if(y_velocity < MAX_FALL_SPEED) {
                y_velocity++;
            }
        }
    }

    // Move and handle collision
    UINT8 hit = TranslateSprite(THIS, 0, y_velocity);

    if(hit != 0) {
        if(y_velocity > 0) {
            on_ground = 1;
        }
        y_velocity = 0;
    } else if(y_velocity != 0) {
        on_ground = 0;
    }

    if(keys == 0) {
        SetSpriteAnim(THIS, anim_idle, 5);
    }
}

void DESTROY() {
}