# sketchtrial

To bypass Sketch trial verification process you just need to spoof a couple of NSDate calls when the app starts. This repository contains a shared library code that can be inserted (using `DYLD_INSERT_LIBRARIES`) to the Sketch process that spoofs the right `gettimeofday` calls.

## Building
```
git clone https://github.com/gch1p/sketchtrial
cd sketchtrial
make
```

## Launching Sketch
You can use `sketch.sh`, it's a wrapper script that sets necessary environment variables to insert the lib and launches Sketch (it assumes that Sketch.app installed to /Applications, edit the script if it differs for you).

## Create Automator script like App (Running with administrator privileges)

*  Open Automator (use Finder)
*  Select New => Application (Set the name for the app, in my case "Sketchv2"
*  Under Actions Search Bar, select Run AppleScript and drag to the right panel (Or double click)
*  Add this to the text area

  ```do shell script "sudo /PATHTOTHISREPO/sketchtrial/sketch.sh" with administrator privileges```
*  You can press the play button to test.
*  Click on File => Convert To => Application
*  Move the new Application to "Applications" folder
*  You can add the new application to your docker
*  You can change the icon Following this link https://9to5mac.com/2019/01/17/change-mac-icons/
 

## Important
This hack doesn't work on systems with SIP (System Integrity Protection) enabled, and it's enabled by default since El Capitan. It can be easily disabled from Recovery Mode but it's another layer of security which is always good to have so disabling it permanently is not the wisest idea. I'll implement some solution to bypass this restriction when I have time.
