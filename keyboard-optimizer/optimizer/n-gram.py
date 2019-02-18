import sys
import re

chrlist = [chr(i) for i in range(97, 97+26)]
ngram = {}
for x in chrlist:
  for y in chrlist:
    ngram[x+y] = 0

pattern = r"wiki_\d+"
filename_pattern = re.compile(pattern)
pattern = r"[a-z]{2}"
chr_pattern = re.compile(pattern)

for filename in sys.argv:
  if filename_pattern.search(filename) == None:
    print("Invalid file:%s" % (filename))
    continue
  
  file_data = open(filename, "r");
  print("File loaded.")
  data = file_data.read();
  file_data.close();

  print("Processing...")

  for i in range(len(data) - 1):
    chrpair = data[i:i+2]
    if chr_pattern.match(chrpair) == None:
      print("Escaped")
      continue
    ngram[chrpair] += 1

  print("Finish!")

print("---------data--------------------------------------")
print(ngram)
print("---------------------------------------------------")

chr_sum = sum(ngram.values())
print("Count:%d" % (chr_sum))
if chr_sum == 0:
  exit()

file_data = open("frequency_data.txt", "w")
for key in ngram.keys():
  file_data.write("%s %s %f\n" % (key[0], key[1], ngram[key] / chr_sum))

  
