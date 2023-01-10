FLAPPY BIRD

x, y position
dx, dy velocity
d2x, d2y acceleration

-- use the x components

- gravity 
- user press key - acceleration on oposite side of gravity
- bird on fixed position x

- divide the visible space on 4 vertical segments (and one extra outside the console)
- store the lower half of the obstacle, the other half can be obtained per calculation
- move the sections to the left - create movement
- get the section off the screen from the front of the list and move it to the back of the list
- reset section offset
- with this - easy to store an infinit world