var PORT_C = 33334;
var PORT_PYTHON_RECV = 33335;
var PORT_PYTHON_SEND = 33336;
var HOST = '127.0.0.1';
var dgram = require('dgram');

var gesture = dgram.createSocket('udp4');

gesture.on('listening', function() {
    var address = gesture.address();
    console.log('Gesture UDP Server listening on ' + address.address + ':' + address.port)
    multiplex.send('START', (HOST, PORT_PYTHON_SEND));
});

sendToMux = true;
gesture.on('message', function (message, remote) {
    gesture = message.byteLength;
    if (gesture == 0) {
        return;
    }
    multiplex.send('STOP', (HOST, PORT_PYTHON_SEND));
    sendToMux = false;
    console.log('Gesture');
    processData(message.byteLength)
    setTimeout(function() {
        sendToMux = true;
    }, 500);
    multiplex.send('START', (HOST, PORT_PYTHON_SEND));
});

gesture.on('error', (err) => {
    console.log('server error:\n${err.stack}');  
//    gesture.close();
//    gesture.bind(PORT_PYTHON_RECV, HOST);
});

gesture.bind(PORT_PYTHON_RECV, HOST);

var keypress = dgram.createSocket('udp4');
var multiplex = dgram.createSocket('udp4');

keypress.on('listening', function() {
    var address = keypress.address();
    console.log('Keypress UDP Server listening on ' + address.address + ':' + address.port)
});


const SHORTCUT = 0;
const EDIT = 1;
const NUM_OF_MODES = 2;
let gestMode = SHORTCUT;

let prevRawCmd = '';
let prevGest = 0;
const currCmd = {
    key: '',
    mode: ''
}

let clearCmd = (rawCmd) => {
    setTimeout(function() {
        prevRawCmd = rawCmd;
        currCmd.key = '';
        currCmd.mode = '';
    }, 1000);
}

keypress.on('message', function (message, remote) {
    if (!sendToMux) {
        //console.log("Don't send!");
        return;
    }
    console.log('Keypress');
    rawCmd = message.toString('utf8');
    if (rawCmd == prevRawCmd) {
        return;
    }
    clearCmd(rawCmd);
    prevRawCmd = rawCmd;
    rawCmd = rawCmd.split('');
    currCmd.mode = rawCmd.splice(0,1)[0];
    currCmd.key = rawCmd.join('');
    ev = (currCmd.mode == 'u') ? 'up' : 'down';
    console.log('key: ' + currCmd.key + ', event: ' + ev);
    keycode = getKeyCode(currCmd.key).toString();
    bLength = Buffer.byteLength(keycode, 'utf8');
    keycode = (bLength == 1) ? '0' + keycode : keycode;
    multiplex.send(keycode.toString(), (HOST, PORT_PYTHON_SEND));
});

keypress.bind(PORT_C, HOST);

const mods = ['Shift', 'Command', 'Option', 'FN', 'Delete', 'Ctrl'];
const vowels = ['a', 'e', 'i', 'o', 'u'];

let processData = (data) => {
    console.log(currCmd, data);
    prevGest = data;
    switch (gestMode) {
        case SHORTCUT:
            shortcutProcess(data);
            break;
        case EDIT:
            editProcess(data);
            break;
        default:
            console.log("Unknown gesture mode");
    }
}

let getKeyCode = (string) => {
    switch (string) {
        case 'a': return 0;
        case 's': return 1;
        case 'd': return 2;
        case 'f': return 3;
        case 'h': return 4;
        case 'g': return 5;
        case 'z': return 6;
        case 'x': return 7;
        case 'c': return 8;
        case 'v': return 9;
        case 'b': return 11;
        case 'q': return 12;
        case 'w': return 13;
        case 'e': return 14;
        case 'r': return 15;
        case 'y': return 16;
        case 't': return 17;
        case 'o': return 31;
        case 'u': return 32;
        case 'i': return 34;
        case 'p': return 35;
        case 'l': return 37;
        case 'j': return 38;
        case 'k': return 40;
        case 'n': return 45;
        case 'm': return 46;
        case 'Delete': return 51;
        case 'Ctrl': return 59;
        case 'FN': return 63;
        default: return -1;
    }
}

let lastGreekLetter = ' ';
let greekLetter = () => {
    runJxa.sync((keyCode) => {
        let se = Application('System Events');
        se.keyCode(51);
        ui = se.applicationProcesses['SystemUIServer'];
        //Τοmás
        // textInput = ui.menuBars[0].menuBarItems[6];
        //Yilei
        textInput = ui.menuBars[0].menuBarItems[4];
        textInput.menus[0].menuItems['Greek'].click()
        textInput.click();
        delay(0.2);
        se.keyCode(keyCode);
        delay(0.2);
        textInput.menus[0].menuItems['U.S.'].click();
        textInput.click();
    }, [getKeyCode(currCmd.key)]);
}

let shortcutProcess = (data) => {
    console.log('shortcut process');
    switch (data) {
        case 1: //UP
            if (currCmd.key == 'm') {
                getTrack();
            } else if (currCmd.key == 'Delete') {
                deelete('up');
            } else if (currCmd.key == 'y') {
                openLink('http://youtube.com');
            } else if (currCmd.key == 'g') {
                openLink('http://google.com');
            } else if (currCmd.key == 'k') {
                openApp('Keynote');
            }
            break;
        case 2: //DOWN
            if (currCmd.key == 'm') {
                addToLibrary();
            } else if (currCmd.key == 'Delete') {
                deelete('down');
            } else if (currCmd.key == 'Ctrl') {
                changeMode();    
            }
            break;
        case 3: //LEFT
            if (currCmd.key == 'Delete') {
                deelete('left');
            }
            break;
        case 4: //RIGHT
            if (currCmd.key == 'Delete') {
                deelete('right');
            } 
            //nextTrack();
            break;
        case 9:
            if (currCmd.key == 'y') {
                doFaceTime('Yilei Shi'); 
            } else if (currCmd.key == 't') {
                doFaceTime('Tomás Vega'); 
            } else if (currCmd.key == 'f') {
                doFullscreen();
            }
            break;
        default:
            console.log('Not implemented');
    }
}

let editProcess = (data) => {
    console.log('edit process');
    switch (data) {
        case 1: //UP
            console.log("UP");
            console.log(currCmd);
            console.log("---------");
            if (currCmd.key == 'Delete') {
                deelete('up');
            } else if (!mods.includes(currCmd.key)) {
                superscript(currCmd.key);
            }
            break;
        case 2: //DOWN
            console.log("DOWN");
            console.log(currCmd);
            console.log("---------");
            if (currCmd.key == 'Delete') {
                deelete('down');
            } else if (!mods.includes(currCmd.key)) {
                subscript(currCmd.key);
            } else if (currCmd.key == 'Ctrl') {
                changeMode();    
            }
            break;
        case 3: //LEFT
            console.log("LEFT");
            console.log(currCmd);
            console.log("---------");
            if (currCmd.key == 'Delete') {
                deelete('left');
            }
            break;
        case 4: //RIGHT
            console.log("RIGHT");
            console.log(currCmd);
            console.log("---------");
            if(vowels.includes(currCmd.key)) {
                doAccent(data);
            } else if (currCmd.key == 'Delete') {
                deelete('right');
            }
            break;
        case 5:
            if (vowels.includes(currCmd.key)) {
                doAccent(data);
            } 
            break;
        case 6:
            if (vowels.includes(currCmd.key)) {
                doAccent(data);
            } 
            break;
        case 7:
            if (vowels.includes(currCmd.key)) {
                doAccent(data);
            } 
            break;
        case 8:
            if (currCmd.key == 'a' || 
                currCmd.key == 'o' || 
                currCmd.key == 'n') {
                doAccent(data);
            } 
            break;
        case 9: //CIRCLE
            console.log("CIRCLE");
            console.log(currCmd);
            console.log("---------");
            if (!mods.includes(currCmd.key)) {
                greekLetter();
            }
        default:
            console.log('Not implemented');
    }
}

let changeMode = () => {
    console.log('changeMode');
    console.log(gestMode);
    gestMode = (gestMode + 1) % NUM_OF_MODES;
    console.log(gestMode);
    theMode = '';
    if (gestMode == SHORTCUT) {
        theMode = 'Shortcut';
    } else {
        theMode = 'Edit';
    }
    notify('Gest-a-Key', '', theMode + ' Mode');
    console.log(gestMode);
}

let doFullscreen = () => {
    runJxa.sync(() => {
        let se = Application('System Events');
        se.keyCode(51);
        se.keystroke('f', { using: [ 'command down', 'control down' ] });
    });
}

let openApp = (appName) => {
    console.log('openApp');
    runJxa.sync((appName) => {
        Application(appName).activate();
    }, [appName]);
    console.log('after open app');

}

let openLink = (url) => {
    runJxa.sync((url) => {
        let se = Application('System Events');
        se.keyCode(51);
        var safari = Application('Safari');
        safari.includeStandardAdditions = true;
        running = safari.running();
        safari.activate(); // Start Safari and bring to front  
        w = safari.windows();
        if (w.length == 0) {
            safari.Document().make();
        } else if (running) {
            w = safari.windows[0];                                                      tabs = w.tabs;
            iTab = tabs.push(new safari.Tab())-1;
            oTab = tabs[iTab];
            w.currentTab = oTab;
        }   
        doc = safari.documents[0];
        doc.url = url; 
    }, [url]);
}

let doFaceTime = (name) => {
    runJxa.sync((name) => {
        s = Application("System Events");
        f = Application("FaceTime");
        f.quit();
        f.activate();
        rows = s.applicationProcesses["FaceTime"].windows['FaceTime'].scrollAreas[0].tables[0].rows;
        i = 1;
        for (i = 0; i < rows().length; i++ ) {
            contactName = rows[i].uiElements[0].staticTexts[0].name();
            console.log(contactName);
            if (contactName == name) {
                break;
            }
        }
        button = rows[i].uiElements[0].buttons[0];
        button.click();
    }, [name]);
}

let getCurrAppName = () => {
    appName = runJxa.sync(() => {
        se = Application("System Events");
        app = se.processes.whose({frontmost: true}).processes.at(0);
        return [app.displayedName()];
    }, []);
    console.log(appName[0]);
    return appName[0];
}

let addToLibrary = () => {
    runJxa(() => {
        currApp = Application.currentApplication();
        iTunes = Application("iTunes");
        iTunes.activate();
        currentTrack = iTunes.currentTrack.name();
        
        rows = Application("System Events").applicationProcesses.byName("iTunes").windows.byName("iTunes").groups[0].groups[0].scrollAreas[0].uiElements[0].tables[0].rows;
        i = 1;
        for (i = 1; i < rows().length; i++ ) {
            trackName = rows[i].uiElements[2].groups[0].staticTexts[0].name();
            if (trackName == currentTrack) {
                break;
            }
        }
        button = rows[i].uiElements[5].groups[0].buttons[0];
        notification = button.description().split(' ');
        console.log(notification);
        action = notification.splice(0, 1)[0];
        if (action == 'Add') {
            action = 'added';
        } else if (action == 'Download') {
            action = 'downloaded';
        } else {
            action = 'undefined';
        }
        if (action != 'undefined') {
            button.click();
        }
        currApp.activate();
        return [action, notification];
    }, []).then(result => {
        action = result[0];
        notification = result[1];
        if (action == 'undefined') {
            song = notification.splice(3).join(' ');
            notify('Gest-a-Key', '', song + ' already in library');
            return; 
        }
        notification.splice(notification.length - 2, 0, action);
        notify('Gest-a-Key', '', notification.join(' '));
    });
}

const SCRIPT_DELAY = 0.2;

let superscript = (letter) => {
    appName = getCurrAppName();
    if (appName != 'Microsoft Word') {
        return;
    }
    console.log(letter);
    console.log('superscript');
    runJxa.sync((letter, SCRIPT_DELAY) => {
        let se = Application('System Events');
        w = se.applicationProcesses['Microsoft Word'];
        //Tomás
        // s = w.windows['Document1'].tabGroups[0].groups[0].checkboxes['Superscript'];
        //Yilei
        s = w.windows['Document1'].tabGroups[0].scrollAreas[0].groups[1].checkboxes['Superscript'];
        se.keyCode(51);
        s.click();
        se.keystroke(letter);
        s.click();
        // se.keyCode(51);
        // delay(SCRIPT_DELAY);
        // se.keystroke('+', { using: [ 'command down', 'shift down' ] });
        // delay(SCRIPT_DELAY);
        // se.keystroke(letter);
        // delay(SCRIPT_DELAY);
        // se.keystroke('+', { using: [ 'command down', 'shift down' ] });
    }, [letter, SCRIPT_DELAY]);
}

let subscript = (letter) => {
    if (getCurrAppName() != 'Microsoft Word') {
        return;
    }
    console.log(currCmd);
    console.log('subscript');
    runJxa.sync((letter, SCRIPT_DELAY) => {
        let se = Application('System Events');

        w = se.applicationProcesses['Microsoft Word'];
        // Tomás
        // s = w.windows['Document1'].tabGroups[0].groups[0].checkboxes['Subscript'];
        // Yilei
        s = w.windows['Document1'].tabGroups[0].scrollAreas[0].groups[1].checkboxes['Subscript'];
        se.keyCode(51);
        s.click();
        se.keystroke(letter);
        s.click();
        // se.keyCode(51);
        // se.keystroke('+', { using: 'command down' });
        // delay(SCRIPT_DELAY);
        // se.keystroke(letter);
        // delay(SCRIPT_DELAY);
        // se.keystroke('+', { using: 'command down' });
    }, [letter, SCRIPT_DELAY]);
}

let deelete = (dir) => {
    runJxa.sync((dir) => {
        let se = Application('System Events');
        switch (dir) {
            case 'right':
                se.keyCode(117, { using: "command down" });
                break;
            case 'up':
                se.keyCode(124, { using: [ "command down", "shift down" ] });
                break;
            case 'left':
                se.keyCode(51, { using: "option down" });
                break;
            case 'down':
                se.keyCode(123, { using: [ "command down", "shift down" ] });
                se.keyCode(51);
                //se.keyCode(51, { using: "command down" });
                break;
            default:
                console.log("Delete not implemented");
        }
    }, [dir]);
    
}

const accents = {
    4: 'u',
    5: 'e',
    6: '`',
    7: 'i',
    8: 'n' 
}

let doAccent = (accent) => {
    console.log('doAccent');
    runJxa.sync((vowel, accent, accents) => {
        let se = Application('System Events');
        se.keyCode(51);
        se.keystroke(accents[accent], { using: 'option down' });
        se.keystroke(vowel);
    }, [currCmd.key, accent, accents]);
};

const runJxa = require('run-jxa');

let getTrack = () => {
    console.log('getTrack');
    runJxa(() => {
        let track = Application('iTunes').currentTrack;
        return [track.name(), track.artist()];
    }, []).then(result => {
        console.log(result);
        notify('Gest-a-Key', result[0], result[1]);
    });
};

let nextTrack = () => {
    runJxa.sync(() => {
        let iTunes = Application('iTunes');
        iTunes.includeStandardAdditions = true;
        return iTunes.nextTrack();
    });
};

let prevTrack = () => {
    runJxa.sync(() => {
        let iTunes = Application('iTunes');
        iTunes.includeStandardAdditions = true;
        return iTunes.previousTrack();
    });
};

let notify = (title, subtitle, text) => {
    runJxa.sync((title, subtitle, text) => {
        currentApp = Application.currentApplication();
        currentApp.includeStandardAdditions = true;
        currentApp.displayNotification(text, 
            { withTitle: title, subtitle: subtitle });
    }, [title, subtitle, text]);
};

