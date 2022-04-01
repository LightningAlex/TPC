# Ragdoll

> NOTE: THE RAGDOLL/KNOCKBACK SYSTEM IS NOT COMPLETED! 

## Introduction
The ragdoll system enables realistic knockback, falling, launching and reacting to hits. It can be activated at any time for any reason. Right now, it is potentially activated when a character gets hit (depending on the force of the attack). 

## Setting up
The ragdoll system exists on the `ATPCharacter` class. In order to make ragdolling possible, some variables need to be set up on the character. 

First of all, the character mesh tick group should be set to "post physics". The anim instance used for the mesh must inherit from `UTPAnimInstance`. The anim graph must have the following nodes:
[![Ragdoll anim graph](https://www.dropbox.com/s/sz45e47nofydknn/RagdollAnimGraph.png?dl=0 "Ragdoll anim graph")](https://www.dropbox.com/s/sz45e47nofydknn/RagdollAnimGraph.png?dl=0 "Ragdoll anim graph")

The "Transform (Modify) Bone" node should modify the root bone, ignore translation and scale, and use "Replace existing" for rotation, in world space.

Furthermore, the following variables need to be set up:
- Get Up Montages: Static array of getting up montages, differentiated by the direction (currently only "forward" and "backward"). No root motion!;
- Ragdoll Alpha Curves: Static array of curves that should be as long as their corresponding montages, and should go from 1 to 0. This curve dictates the blending between the final ragdoll pose and the actual animation;
- Root Bone Name: Self-explanatory;
- Ragdoll Face Bone Name: The bone that is used to determine the direction the character body is facing. Usually "pelvis";
- Skeleton root child: The bone connected to the root bone that most, if not all, other bone chains start from. Usually "pelvis";
- Mesh forward vector: The vector that points "in front" of the mesh, in component space. Usually "Y";
- Skeleton Root Child Actual Forward: The vector that points "in front" of the skeleton root child bone (described above) in bone space. Usually "Y";
- Mesh Forward Inverted: Boolean determining whether the above mentioned mesh forward vector should be inverted (i.e. -Y instead of Y);
- Skeleton Root Child Inverted: Same as `MeshForwardInverted`, but for the skeleton root child;
- Disable Recovery: If this is true, a character can never recover from ragdolling;
> Author's note: I have coded the ragdoll system long before writing this documentation - I can't figure out why the last variable is needed. It is not blueprintwriteable. It's value isn't changed anywhere, not even when a character's health reaches 0. 