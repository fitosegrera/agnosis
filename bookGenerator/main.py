from clarifai.client import ClarifaiApi
import urllib2
import urllib
import random
import ast
import sys
import json

path = sys.argv[1]
print "path to process:", path
outPath = "output/" + sys.argv[2]
print "data will be saved at:", outPath

#Process image with clarifai
def processClarifai(imgUrl):
	clarifai_api = ClarifaiApi("3RJprl1ls24WbI4X4FAfJe9zOL5TO_ogBU4druEW", "A7i9vHq95Kck9lbgsWZ_s4ZuLkUQBfL_iWE3J5Kt")  # assumes environment variables are set.
	result = clarifai_api.tag_image_urls(imgUrl)
	classes = result['results'][0]['result']['tag']['classes']
	total = []
	for c in classes:
		total.append(c)
	return total

#Get google autocomplete
def googleAutocomplete(q):
	query = q
	url = "http://suggestqueries.google.com/complete/search?client=firefox&q="+query
	result = urllib2.urlopen(url).read()
	return result

#Parse google's suggested data
def parseData(d, t):
	d = ast.literal_eval(d)
	resultPhrase = ""
	if len(d[1]) > 0:
		index = random.randint(0, len(d[1])-1)
		resultPhrase = d[1][index]
	else:
		resultPhrase = t
	return resultPhrase

#Generate an action
def getAction():
	actions = [
		"I saw ",
		"I see ",
		"seeing ",
		"I walked "
		"walking ",
		"I walk ",
		"walker ",
		"pedestrian "
		"I observe ",
		"I observed ",
		"observer"
		"observing ",
		"I was ",
		"I am ",
		"I will be ",
		"he is ",
		"he was ",
		"he will be ",
		"she is ", 
		"she was ",
		"she will be ",
		"I took ",
		"I wonder "
	]

	choiceIndex = random.randint(0, len(actions)-1)
	choice = actions[choiceIndex]
	return choice

def getJsonURL(u, poem):
	result = urllib2.urlopen(u).read()
	result = json.loads(result)
	output = {
		"attention": result['attention'],
		"gps": {
			"lon": result['gps']['lon'],
			"lat": result['gps']['lat'],
			"alt": result['gps']['alt']
		},
		"accelerometer": {
			"z": result['accelerometer']['z'],
			"x": result['accelerometer']['x'],
			"y": result['accelerometer']['y']
		},
		"image_path": result['image_path'],
		"timestamp": result['timestamp'],
		"raw": result['raw'],
		"orientation": {
			"z": result['orientation']['z'],
			"x": result['orientation']['x'],
			"y": result['orientation']['y']
		},
		"meditation": result['meditation'],
		"poem": poem
	}

	return output

#Main function
if __name__ == "__main__":

	totalMemories = int(sys.argv[3])
	
	for i in range(totalMemories):
		print "processing image", i+1
		tags = processClarifai(path + str(i+1) +  '.jpg')
		print "geting data from google..."
		print "--------------------------"
		lines = ""
		for tag in tags:
			action = getAction()
			toSearch = action + tag
			toSearch.encode('utf-8')
			toSearch = urllib.quote(toSearch)
			search = googleAutocomplete(toSearch)
			resultPhrase = parseData(search, tag)
			lines += resultPhrase + ', '

		output = getJsonURL(path + str(i+1) + ".txt", lines)

		with open(outPath + str(i+1) + '.txt', 'w') as outfile:
			json.dump(output, outfile, indent=4, sort_keys=True)
			print "DATA SAVED!\n"