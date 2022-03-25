# Equipment
## Description
The equipment system allows any character to equip any amount of equipment items. Equipment items encompass armaments (weapons and shields), armor and attachments (like rings and talismans). 
Every equipment item can increase/decrease any combat stats (see stats for more info).
> NOTE: In the future, we'd like to enable percentual increases/decreases

Under the hood, there are only two types of equipment: static mesh equipment and skeletal mesh equipment.

## Classes and functions
The base class for equipment items is `ATPEquipmentBase`, which inherits from `AActor`. It is an abstract class. Equipping and unequipping items is handled in the combat component. When an item is equipped, `BeEquipped()` is called. Similarly, when an item is unequipped, `BeUnequipped` is called. Both functions are part of `ATPEquipmentBase`. The base functions handle the increasing/decreasing of stats, and should always be called in their overridden version. Aside the base functions, two similarly named blueprint implementable events are called.

There are two classes that inherit from `ATPEquipmentBase`, namely `ATPArmorBase` and `ATPWeaponBase`. 
> NOTE: The word "weapon" should be renamed to "armament", given that the base weapon class encompasses shields, as well. 

### Differences
The main difference between weapons and armor is that weapons use a static mesh, and armor uses a skeletal mesh. Furthermore, weapons are usually attached to a socket, but armor isn't. It is worth pointing out that the socket info is in the base class, so it is possible to attach a piece of armor to a socket - though it's not recommended in any way. 
The other big difference is that weapons can be used to deal damage and to actively block incoming attacks. Weapons can have an element (see stats for more info) imbued into them. 

## Equipment
