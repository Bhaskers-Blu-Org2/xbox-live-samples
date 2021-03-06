# Xbox Live iOS Native Sample App


## Overview

The iOS Sample App can be found in the Xbox Live Samples repository, in 
`xbox-live-samples/MobileSDK/Samples-Native/Sample-iOS`.
The project is already set up for the Xbox Live libraries (frameworks).

To learn how to set up an iOS project for Xbox Live go to 
[Setting up Xcode targeting iOS](https://docs.microsoft.com/en-us/gaming/xbox-live/get-started/setup-ide/managed-partners/xcode-ios/xcode-ios-mp) 
(https://docs.microsoft.com/en-us/gaming/xbox-live/get-started/setup-ide/managed-partners/xcode-ios/xcode-ios-mp).

**Sample.xcodeproj** is the app's project file, and can be opened in Xcode. Almost all the source code 
is in the Samples directory. Most of the project groups match subdirectories of the Samples 
directory.

The main view has the following three sections:
* Top = Identity View - sign-in/out, signed-in user's profile.
* Middle = Hub Menu - menus are displayed here.
* Bottom = Screen Log - a scrollable text view displaying logging messages.


## Core Project

The iOS sample app was built off of Xcode's Single View Controller template for Objective-C.
* App entry points (**main.m**, **AppDelegate.h/.m**) - these are boilerplate, from the template. Also standard 
**info.plist** (already set up for XBL), and **Launch.storyboard** set up with a simple launch view layout.
	
* **RootViewController.h/.mm**, **Main.storyboard**: The main view controller of the app, with the main view 
defined in **Main.storyboard**. Class is defined as Objective-C, but implemented as Objective-C++ (*.mm), 
so it can call C++ methods.
		
* The Root View is setup in **Main.storyboard**, using Xcode's Interface Builder tools.

* **pch.h**, the app's precompiled header. This is set in the project's build settings as the Prefix Header, 
and this header file is automatically included in all source files in the project. It has C++ and Obj-C 
sections (*.mm files thus load both).
	
Other source subdirectories are described below.
		

## Utilities

Useful utilities.
* The screen logger: TextViewLog.h/.m - a simple text view logger, using a UITextView, and 
ScreenLog.h/.mm - a C/C++ wrapper around the TextViewLog Objective-C class.
	
* salieri.h - Defines the Microsoft Source Annotation Language (SAL), used in the Xbox Live 
frameworks and some C/C++ code in the iOS sample app. For Xcode, which does not support SAL, 
this file makes sure all the annotations are ignored. This is a public file from <salieri github>.


## Integration

The Integration files are the only source files found outside of the iOS project directory. They can 
be found at `xbox-live-samples/MobileSDK/Integration`. These files are cross-platform, and shared by 
all of the iOS and Android sample apps.

The Integration files are a C++ wrapper around the Xbox Live C defined libraries. In general, these 
files should not be changed, and the Xcode project does not let you open them, though you can still 
open and edit them directly through the file system.

The integration files currently cover the following Xbox Live services: Identity (Sign-In/Out, User 
Profile), Achievements, and Social.

The integration files also define and call several Gameplay named C functions, that each app using these 
files needs to implement.


## Gameplay

The C++ files here implement the gameplay C functions defined in the cross-platform integration files.
In general, these gameplay functions are called by Xbox Live services (usually called from asynchronous 
responses to XBL requests), which then call C++ methods in the **Game_Integration** subdirectory.


## Game_Integration

These C++ singleton classes provide communication between the Xbox Live services and the sample app's 
display views. Most of these classes are implemented as Objective-C++ (.mm) files, so they can call 
Objective-C classes.
* **Game_Integration.h/.cpp** - Xbox Live initialization, plus context and user access once signed in.
	
* **HubMenu_Integration.h/.mm** - Integrates with **HubMenuView**. This is the base menu that displays once 
the user is signed in, with a button for each XBL service to sample (so far, Achievements, and Social). All 
other menu views are layered on top of this view as subviews.
	
Other menu integration files are in subdirectories by Xbox Live service, and named to match the Objective-C 
UIView based menu view each integrates with (Identity, Achievements, and Social, so far).


## Views

The display and menu views for the iOS sample app. These are Objective-C classes extending the iOS standard 
**UIView** class (or a standard subclass, such as **UITableView**). They are all implemented as Objective-C++ 
(.mm) files, so they can call C++ methods. The views are in subdirectories matching Xbox Live service type.
	
Each view consists of 3 files: an Objective-C class header (.h), an Objective-C++ class implementation file 
(.mm), and an Interface Builder file (.xib) defining the view's layout.
	
Menu views that integrate with Xbox Live C++ classes will attach themselves to their matching C++ singletons 
on initialization, and will detach on menu exit.
