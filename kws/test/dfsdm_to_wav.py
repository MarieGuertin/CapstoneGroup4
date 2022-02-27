#!/usr/bin/python

import sys, getopt

import scipy
from scipy import io
from scipy.io import wavfile

def main(argv):
   inputfile = ''
   outputfile = ''
   try:
      opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
   except getopt.GetoptError:
      print('dfsdm_to_wav.py -i <inputfile.txt> -o <outputfile.wav>')
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print('dfsdm_to_wav.py -i <inputfile.txt> -o <outputfile.wav>')
         sys.exit()
      elif opt in ("-i", "--ifile"):
         inputfile = arg
      elif opt in ("-o", "--ofile"):
         outputfile = arg

   with open(inputfile) as file:
      dfsdm_output = list()
      for line in file:
         for word in line.split(','):
            word = word.strip()
            if word.isnumeric() or (word.startswith("-") and word[1:].isnumeric()):
               dfsdm_output.append(word)

   import numpy as np
   wavfile.write(outputfile, 16000, np.array(dfsdm_output).astype(np.int16))


if __name__ == "__main__":
   main(sys.argv[1:])