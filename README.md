# agnosis
###Author: fito_segrera
###2016

###This repository contains the 3 main applications which form the system AGNOSIS: The lost memories.

* agnosis_sculptures: an openframeworks application which renders 3D objects from the images and .txt (json strings) generated by the phoneAppBCI android app.

* phoneAppBCI: android app which interfaces with neurosky's mindwave scanner. It is programmed to take a picture and read some of the phone's sensor data when the attention level is bellow certain threshold.

* bookGenerator: generates a dual page with a poem composed from the interpretation of the image. It uses clarifai' image recognition API to get word-tags from each memory (image) and the google suggestion API to create phrases from the word-tags. The data is then rendered as a memory log on a dual page; part of a book of lost memories.

### INSTRUCTIONS:

#### 3.bookGenerator:

cd into the app's folder

	cd bookGenerator

execute the program giving the right arguments

	python main.py URL/PATH/FOR/DATA LOCAL/OUTPUT/PATH totalNumberOfMemories

This application assumes you have all your images and texts produced by the phoneAppBCI program uploaded to a server and publicly accessible; mine are located at: http://fii.to/agnosis. In my case I would run the bookGenerator app like this: 

	python main.py http://fii.to/agnosis_memories/shanghai-28-08-16/1/memory_ shanghai-28-08-16/1/memory_ 40

The new .txt files containing the data from the memories will be located at your LOCAL/OUTPUT/PATH