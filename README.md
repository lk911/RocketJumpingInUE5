# Rocket Jumping in UE5

Trying to recreate the rocket jump from Team Fortress 2 in Unreal Engine 5, written in C++.

## What works so far

- First-person character with movement and jumping (Enhanced Input)
- Clicking fires a rocket from the camera in the direction you're aiming
- Rockets use UProjectileMovementComponent and get destroyed when they hit something
- Rockets explode on impact and push nearby characters away from the blast, strongest at the center and fading toward the edge like TF2

## To do

- Explosion effects and sound

## Running it

Built with Unreal Engine 5.6. Open `RocketJumpGame.uproject`, let it compile the C++ module, and hit Play.

## Code

- `Jumper` - the player character (movement, jump, shooting)
- `Rocket` - the projectile, collision, and launching the player
- `jumperPC` - player controller
