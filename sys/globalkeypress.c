#include <ApplicationServices/ApplicationServices.h>
#include <stdio.h>

#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 33334   //The port on which to send data

struct sockaddr_in si_other;
int s, i, slen=sizeof(si_other);
char buf[BUFLEN];
char message[BUFLEN];

void bye(void) {
    close(s); 
}

void die(char *s)
{
    perror(s);
    exit(1);
}

char * convertKeyCode(int keyCode)
{
    printf("keyCode: %d\n", keyCode);
    // Proper key detection seems to want a switch statement, unfortunately
    switch (keyCode) {
        case 0: return("a");
        case 1: return("s");
        case 2: return("d");
        case 3: return("f");
        case 4: return("h");
        case 5: return("g");
        case 6: return("z");
        case 7: return("x");
        case 8: return("c");
        case 9: return("v");
                // what is 10?
        case 11: return("b");
        case 12: return("q");
        case 13: return("w");
        case 14: return("e");
        case 15: return("r");
        case 16: return("y");
        case 17: return("t");
        case 18: return("1");
        case 19: return("2");
        case 20: return("3");
        case 21: return("4");
        case 22: return("6");
        case 23: return("5");
        case 24: return("=");
        case 25: return("9");
        case 26: return("7");
        case 27: return("-");
        case 28: return("8");
        case 29: return("0");
        case 30: return("]");
        case 31: return("o");
        case 32: return("u");
        case 33: return("[");
        case 34: return("i");
        case 35: return("p");
        case 36: return("Enter");
        case 37: return("l");
        case 38: return("j");
        case 39: return("'");
        case 40: return("k");
        case 41: return(";");
        case 42: return("\\");
        case 43: return(",");
        case 44: return("/");
        case 45: return("n");
        case 46: return("m");
        case 47: return(".");
        case 48: return("Tab");
        case 49: return("Space");
        case 50: return("`");
        case 51: return("Delete");
        case 52: return("Enter");
        case 53: return("ESC");
        case 54: return ("RCommand");//Right
        case 55: return ("LCommand");//Left
        case 56: return ("LShift");//Left
        case 57: return ("CapsLock");
        case 58: return ("LOption");//Left
        case 59: return ("Ctrl");
        case 60: return ("RShift");//Right
        case 61: return ("ROption");//Right
        case 63: return ("FN");
        case 65: return(".");
        case 67: return("*");
        case 69: return("+");
        case 75: return("/");
        case 76: return("Enter");   // numberpad on full kbd
        case 78: return("-");
        case 81: return("=");
        case 82: return("0");
        case 83: return("1");
        case 84: return("2");
        case 85: return("3");
        case 86: return("4");
        case 87: return("5");
        case 88: return("6");
        case 89: return("7");
        case 91: return("8");
        case 92: return("9");
        case 96: return("F5");
        case 97: return("F6");
        case 98: return("F7");
        case 99: return("F3");
        case 100: return("F8");
        case 101: return("F9");
        case 103: return("F11");
        case 105: return("F13");
        case 107: return("F14");
        case 109: return("F10");
        case 111: return("F12");
        case 113: return("F15");
        case 114: return("HELP");
        case 115: return("HOME");
        case 116: return("PGUP");
        case 117: return("DELETE");  // full keyboard right side numberpad
        case 118: return("F4");
        case 119: return("END");
        case 120: return("F2");
        case 121: return("PGDN");
        case 122: return("F1");
        case 123: return("Left");
        case 124: return("Right");
        case 125: return("Down");
        case 126: return("Up");
        case 131: return ("F4");
        case 160: return ("F3");
        default: return ("Unknown");
    }
}

char *convertShiftKeyCode(int keyCode) {
    switch ((int) keyCode) {
        case 0:   return "A";
        case 1:   return "S";
        case 2:   return "D";
        case 3:   return "F";
        case 4:   return "H";
        case 5:   return "G";
        case 6:   return "Z";
        case 7:   return "X";
        case 8:   return "C";
        case 9:   return "V";
        case 11:  return "B";
        case 12:  return "Q";
        case 13:  return "W";
        case 14:  return "E";
        case 15:  return "R";
        case 16:  return "Y";
        case 17:  return "T";
        case 18:  return "!";
        case 19:  return "@";
        case 20:  return "#";
        case 21:  return "$";
        case 22:  return "^";
        case 23:  return "%";
        case 24:  return "+";
        case 25:  return "(";
        case 26:  return "&";
        case 27:  return "_";
        case 28:  return "*";
        case 29:  return ")";
        case 30:  return "}";
        case 31:  return "O";
        case 32:  return "U";
        case 33:  return "{";
        case 34:  return "I";
        case 35:  return "P";
        case 37:  return "L";
        case 38:  return "J";
        case 39:  return "\"";
        case 40:  return "K";
        case 41:  return ":";
        case 42:  return "|";
        case 43:  return "<";
        case 44:  return "?";
        case 45:  return "N";
        case 46:  return "M";
        case 47:  return ">";
        case 50:  return "~";
        case 51: return("Delete");
        case 52: return("Enter");
        case 53: return("ESC");
        case 54: return ("RCommand");//Right
        case 55: return ("LCommand");//Left
        case 56: return ("LShift");//Left
        case 57: return ("CapsLock");
        case 58: return ("LOption");//Left
        case 59: return ("Ctrl");
        case 60: return ("RShift");//Right
        case 61: return ("ROption");//Right
        case 63: return ("FN");
        case 65: return(".");
        case 67: return("*");
        case 69: return("+");
        case 75: return("/");
        case 76: return("Enter");   // numberpad on full kbd
        case 78: return("-");
        case 81: return("=");
        case 82: return("0");
        case 83: return("1");
        case 84: return("2");
        case 85: return("3");
        case 86: return("4");
        case 87: return("5");
        case 88: return("6");
        case 89: return("7");
        case 91: return("8");
        case 92: return("9");
        case 96: return("F5");
        case 97: return("F6");
        case 98: return("F7");
        case 99: return("F3");
        case 100: return("F8");
        case 101: return("F9");
        case 103: return("F11");
        case 105: return("F13");
        case 107: return("F14");
        case 109: return("F10");
        case 111: return("F12");
        case 113: return("F15");
        case 114: return("HELP");
        case 115: return("HOME");
        case 116: return("PGUP");
        case 117: return("DELETE");  // full keyboard right side numberpad
        case 118: return("F4");
        case 119: return("END");
        case 120: return("F2");
        case 121: return("PGDN");
        case 122: return("F1");
        case 123: return("Left");
        case 124: return("Right");
        case 125: return("Down");
        case 126: return("Up");
        case 131: return ("F4");
        case 160: return ("F3");
        default: return ("Unknown");
    }
}

bool shiftKeyPressed = false;
bool controlKeyPressed = false;
bool optionKeyPressed = false;
bool commandKeyPressed = false;
bool functionKeyPressed = false;

CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
    // Paranoid sanity check.
    if ((type != kCGEventKeyDown) && 
        (type != kCGEventKeyUp) && 
        (type != kCGEventFlagsChanged)) {
        return event;
    }
    // The incoming keycode.
    CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(
            event, 
            kCGKeyboardEventKeycode
        );

    uint8_t isShiftKey = (keyCode == 56 || keyCode == 60);
    uint8_t isControlKey = (keyCode == 59 || keyCode == 62);
    uint8_t isOptionKey = (keyCode == 58 || keyCode == 61);
    uint8_t isCommandKey = (keyCode == 54 || keyCode == 55);
    uint8_t isFunctionKey = (keyCode == 63);

    if (type == kCGEventFlagsChanged) {
        if (isShiftKey) {
            shiftKeyPressed = !shiftKeyPressed;
        }
        if (isControlKey) {
            controlKeyPressed = !controlKeyPressed;

        }
        if (isOptionKey) {
            optionKeyPressed = !optionKeyPressed;

        }
        if (isCommandKey) {
            commandKeyPressed = !commandKeyPressed;
        }
        if (isFunctionKey) {
            functionKeyPressed = !functionKeyPressed;
        }
    }
    
    // Print the human readable key

    switch(type) {
        case kCGEventKeyDown:
            if (shiftKeyPressed) {
                snprintf(buf, sizeof(buf), "%s%s", "d", convertKeyCode(keyCode));
                //snprintf(buf, sizeof(buf), "%s%s", "d", convertShiftKeyCode(keyCode));
            } else {
                snprintf(buf, sizeof(buf), "%s%s", "d", convertKeyCode(keyCode));
            }
            break;
        case kCGEventKeyUp:
            if (shiftKeyPressed) {
                snprintf(buf, sizeof(buf), "%s%s", "u", convertKeyCode(keyCode));
                //snprintf(buf, sizeof(buf), "%s%s", "u", convertShiftKeyCode(keyCode));
            } else {
                snprintf(buf, sizeof(buf), "%s%s", "u", convertKeyCode(keyCode));
            }
            break;
        case kCGEventFlagsChanged:
            if ((isShiftKey && !shiftKeyPressed) ||
                (isControlKey && !controlKeyPressed) ||
                (isOptionKey && !optionKeyPressed) ||
                (isCommandKey && !commandKeyPressed) ||
                (isFunctionKey && !functionKeyPressed)) {
                //key = "[released %s]\n", convertKeyCode(keyCode));
                snprintf(buf, sizeof(buf), "%s%s", "u", convertKeyCode(keyCode));
            } else {
                snprintf(buf, sizeof(buf), "%s%s", "d", convertKeyCode(keyCode));
                
            }
            break;
        default:
            break;
    }

    //if (shiftKeyPressed && type == kCGEventKeyDown) {
    //    //key = convertShiftKeyCode(keyCode);
    //    snprintf(buf, sizeof(buf), "%s%s", "d", convertShiftKeyCode(keyCode));
    //} else {
    //    if (type != kCGEventKeyUp) {
    //        if ((isShiftKey && !shiftKeyPressed) ||
    //            (isControlKey && !controlKeyPressed) ||
    //            (isOptionKey && !optionKeyPressed) ||
    //            (isCommandKey && !commandKeyPressed)) {
    //            //key = "[released %s]\n", convertKeyCode(keyCode));
    //            snprintf(buf, sizeof(buf), "%s%s", "u", convertKeyCode(keyCode));
    //        } else if (!shiftKeyPressed) {
    //            snprintf(buf, sizeof(buf), "%s%s", "u", convertKeyCode(keyCode));
    //            //printf("%s\n", convertKeyCode(keyCode));
    //        }
    //    }
    //}

    //Keypress code goes here.
    //keyStringForKeyCode(keycode);
    if (sendto(s, buf, strlen(buf) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto()");
    }
    // We must return the event for it to be useful.
    return event;

}


int main(void)
{
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 )
    {
        die("socket");

    }
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);

    }
    
    CFMachPortRef      eventTap;
    CGEventMask        eventMask;
    CFRunLoopSourceRef runLoopSource;

    // Create an event tap. We are interested in key presses.
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp) | (1 << kCGEventFlagsChanged));
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0,
            eventMask, myCGEventCallback, NULL);
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
        exit(1);

    }   
    // Create a run loop source.
    runLoopSource = CFMachPortCreateRunLoopSource(
            kCFAllocatorDefault, eventTap, 0
            );

    // Add to the current run loop.
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,
            kCFRunLoopCommonModes);

    // Enable the event tap.
    CGEventTapEnable(eventTap, true);

    // Set it all running.
    CFRunLoopRun();


    atexit(bye);
    exit(0);
}
