# InterSchem
A simple logic scheme visualizer, allowing the user to create, edit, load and save, export and run different logic schemes.
Made in collaboration with Tudor Bălan.
## Starting InterSchem up
In order to start the program, run the `main.exe` file. This will open a console and another window. The operating the program won't require any console interraction, it is there simply for debbuging.
## Building from source
Currently unavailable, will follow up with a way to build the program from the source code soon. For now, it can only be ran by running the `main.exe` file.
## Using InterSchem
We'll walk you through some of the things you can do in InterSchem.
### Inserting a block <br>
To insert a block, simply click on the `Insert` button. The insert menu will show up on the left hand side where you can select the type of block you wish to insert.
<br>
There are numerous blocks to choose from. A valid logic scheme must always have a `Start` block and a `Stop` block. Other blocks will require an input from the user before being inserted.
<br><br><img src="https://media.giphy.com/media/M5UzxrVjOYOZWUaUfF/giphy.gif"></img>
### Connecting two blocks <br>
In order to connect two blocks, simply click on the circle belonging to the starting block to commence pairing. After that, click on the block you wish to be connected to.
<br>
All blocks may have multiple inputs but only one output per circle.
<br><br><img src="https://media.giphy.com/media/YHS9RdTl12pygJ66rV/giphy.gif"></img>
### Deleting a connection <br>
To delete a connection, right click on the circle of the starting block. Holding down right click will highlight all the connections about to be removed in red.
<br><br><img src="https://media.giphy.com/media/mdvV3L4cUZ3qxa96Ri/giphy.gif"></img>
### Deleting a block <br>
To delete a block, right click on the block. Holding down right click will highlight all of the connections about to be removed when the block will be removed in red.
<br><br><img src="https://media.giphy.com/media/rDWvzHzvK8Zjm46Azk/giphy.gif"></img>
### File and Load <br>
Pressing the `File` button will prompt the user to enter the name of a file. This file will be read from when pressing the `Load` button in order to load a logic scheme (complete or incomplete). InterSchem uses its own file type in order to store information about a logic scheme. By default, `Load` reads from `input.sch`.
<br><br><img src="https://media.giphy.com/media/rP7ag7GzkKoM3JBF4Q/giphy.gif"></img>
### Save <br>
Pressing the `Save` button will encode the scheme into a file that the program can read from later on using `Load`. You can set the name of the file by pressing `File` and writing the name before pressing `Save`. By default `Save` writes to the `input.sch` file.
<br><br><img src="https://media.giphy.com/media/6ac9zOwYYuTqHxwhza/giphy.gif"></img>
### Run <br>
Pressing the `Run` button will prompt the program to run the logic scheme the user has created only if the logic scheme is correct. In order to be correct, a logic scheme must only have one `Start` block, one `Stop` block and all blocks must be connected between each other.
<br><br><img src="https://media.giphy.com/media/4h8gDyTm50mr2unt9M/giphy.gif"></img>

