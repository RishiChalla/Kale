# Islands
This is a doc full of random notes for the game.

## Movement
 - Jump
 	- Typical jumping ability, you start the game with this
 	- You can jump anywhere on the ground, but NOT on walls
 - Shoot
	- Shoots the player in the direction being aimed
	- If no direction is aimed, then it just shoots towards the current velocity direction
	- If the velocity is 0, then it just shoots up
	- Your shoot ability resets when you go on a wall (certain surfaces can avoid this)
 - Super shoot
	- Not only lets you shoot off of the ground and most walls, but also lets you shoot off of most enemies
	- Super shooting restores your jump and bounce
 - Bounce
	- Hold a button to surround the player with a ball, can be used once per jump.
	- While this ball is active, any surfaces touched will be bounced on, this can only be done once then the ball will die out
	- Certain surfaces can't be bounced on
 - Spin
	- Holds on to a point using the fox's tail, and starts spinning/looping around the object held to
	- Spinning like this increases velocity to a certain threshold, can set it up to ram into spikes/etc
	- Spinning restores your jump, shoot, and bounce
 - Restore
	- Restores your jump, shoot, and bounce mid air, this can be done ranged but may pull you into traps/spikes

## Environment
 - Velocity limiters
	- You cannot pass these walls if you are moving too fast
 - Velocity minimum
	- You cannot pass these walls unless you are moving fast enough
 - Spikes
	- Typical spikes, touching these damages you. Can be bounced off of
 - Energyless surfaces
	- Touching these surfaces will not restore the shoot or bounce ability. These surfaces also cannot be bounced on
 - Spin Pulser
	- This is thee center used for the spin ability
 - Locks
	- These doors won't open without having all required keys collected to open them
 - Fast Zones
	- Always locked behind velocity minimums, inside fast zones all velocity adheres to the minimum
 - Levers
	- Does something when flipped
 - Buttons
	- Does something when pressed
 - Tangible Objects
	- Things that can be pushed around to interact with levers/buttons
 - Collectables
	- Things to increase maximum energy, health
	- Things to replenish energy, health
	- Ability fragments - collect all the fragments to unlock a certain ability (this is for both story and combat, certain abilities can be purchased instead with gold)
	- Gold - general collectable. Can be used to purchase certain abilities, upgrade abilities, purchase a certain extent of max health/energy

## Combat
Energy is replenished slowly anytime while in contact with a energetic surface

 - Active
	- Scratching
		- Melee attack, damages in the direction currently moving in. Single target
		- If velocity is 0, then it attacks left/right based on player orientation
	- Sword
		- Melee attack, splash damage in the same direction scratch would be used in. Higher damage than scratch but has a higher recharge
		- Can be purchased with gold
	- Slam
		- Shooting into enemies does a large amount of damage but also has a bit of recoil damage (does damage to yourself). Single target
		- Can be purchased with gold
	- Impact
    	- Uses energy when shooting off of an enemy in order to shoot them at a very high speed in the opposite direction, no cooldown
    	- Can be unlocked with ability fragments
  	- Freeze
    	- Mid range attack, creates a sphere around the player which slows movement of all enemies within the sphere. Uses energy when active
    	- Can be purchased with gold
	- Leech
		- Uses energy to steal a nearby enemy's health, ranged attack. Has a large cooldown
		- Can be unlocked with ability fragments
	- Energy gun
		- Shoots energy in a beam, deals damage to all enemies hit in the beam
		- Upgrades cause it to reflect on surfaces
		- Can be purchased with gold
	- Energy pellets
		- Shoots energy bullets, each bullet deals damage to an individual enemy
		- Reflects on infinite surfaces by default but will expire after a certain amount of time
		- Can be unlocked with ability fragments, can also trigger buttons/levers for other platforming sections
 - Overcharge
	- Has no cooldown, but only lasts as long as 1 second per 10 damage dealt since the last overdrive use. Caps at 15 seconds.
	- When active, damage dealt is 1.5x, abilities replenish 2x faster.
	- Unlocked with ability fragments
 - Passive
	- Motivation
    	- Dealing damage decreases cooldown for the highest cooldown ability by a slight bit
    	- Can be unlocked with ability fragments
  	- Revenge
    	- Deals 1.5x more damage after going below half health
    	- Can be upgraded to also do 2x damage after going below quarter health
    	- Can be purchased with gold
  	- Daredevil
    	- All damage dealt and taken is doubled
    	- Can be purchased with gold

## Story
 - World
	- The setting is in a bunch of floating islands, the floating islands stay afloat due to their vast supply of energy.
 - Intro
	- 