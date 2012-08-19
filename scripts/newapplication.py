#!/usr/bin/python

import sys, os, shutil, fnmatch, fileinput, datetime

'''Creates a new Project based on Section9 Template'''
dtext = "___DATE___"    

basedir = os.getcwd()

classname = "TemplateApp"
namespace = "TemplateNamespace"
cmakename = "template"

if __name__ == "__main__":
  if len(sys.argv) > 1:
    newdir =  basedir + "/../applications/" + sys.argv[1].lower()
    templatedir = basedir + "/../applications/template"

    shutil.copytree(templatedir, newdir)
  
    matches = []
    for root, dirnames, filenames in os.walk(newdir):
      for filename in fnmatch.filter(filenames, '*.cpp'):
        matches.append(os.path.join(root, filename))
      for filename in fnmatch.filter(filenames, '*.hpp'):
        matches.append(os.path.join(root, filename))
      for filename in fnmatch.filter(filenames, 'CMakeLists.txt'):
        matches.append(os.path.join(root, filename))
     
        matches.append(os.path.join(root, filename))

    for line in fileinput.input(matches, inplace=1):
      if classname in line:
        line = line.replace(classname, sys.argv[1].capitalize() + "App")
      if namespace in line:
        line = line.replace(namespace, sys.argv[1].lower())
      if cmakename in line:
        line = line.replace(cmakename, sys.argv[1].lower())
    
      sys.stdout.write(line)

    print "Created new project in " + newdir
  else:
    print "You must specify a name for the project"
