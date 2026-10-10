# Rocket Jumping in UE5

Trying to recreate the rocket jump from Team Fortress 2 in Unreal Engine 5, written in C++.

## What works so far

- First-person character with movement and jumping (Enhanced Input)
- Clicking fires a rocket from the camera in the direction you're aiming
- Rockets use UProjectileMovementComponent and get destroyed when they hit something
- A rocket hitting the player launches them upward

## To do

- Proper radial knockback based on how close the explosion is, like in TF2
- Explosion effects and sound

## Running it

Built with Unreal Engine 5.6. Open `RocketJumpGame.uproject`, let it compile the C++ module, and hit Play.

## Code

- `Jumper` - the player character (movement, jump, shooting)
- `Rocket` - the projectile, collision, and launching the player
- `jumperPC` - player controller
