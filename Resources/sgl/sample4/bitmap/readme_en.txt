◎About this sample

SGL comes with a bitmap drawing function that uses SATURN's bitmap screen.
In the past, when a new computer came out, people used to do it to show off its drawing ability.
Something like that. BASIC often comes with this kind of demo.
So I used these functions to recreate old demos and felt nostalgic (?)
That is the purpose of this program.

★Points to note
The center point of all bitmap display functions is the center of the screen.
Therefore, please keep this in mind when setting coordinates.

★How to operate
First, "Press Start Button" will be displayed on the screen. why are you doing this
The reason for this is that it uses the SGL function slRandom.
This function is initialized with exactly the same base during slInitSystem.
Also, this function updates the base number during slSynch. So after slInitSystem
If the processing is exactly the same, only the same random number sequence will be called.
Here, some calls to slSynch ensure that the random number processing is not the same.
doing.
Now, start button (this sample uses the digital pad directly connected to Port 1 of SATURN).
I have only seen it. ) to display a dot, circle, or square.
Use the pad's cursor keys to move the bitmap screen up, down, left and right.
Press the start button to return to the starting position.
