window.onload = main

var _wW = window.innerWidth;
var _wH = window.innerHeight;

var _input = null;
var _ctx = null;
var _canvas = null;
var _images = new Array();
var _sliders = {
    "brightness" : null,
    "contrast"   : null,
    "saturate"   : null
};


function main() {
    init();
    window.requestAnimationFrame(frameloop);
    window.cancelAnimationFrame(frameloop); 
}

function init() {
    var off = 0;
    for(var i in _sliders) {
        var p = newElement("p", document.body, _wW - 200, (_wH/2) - off, 1, 1, i, null);
        p.textContent = i + ":";
        _sliders[i] = newElement("input", document.body, _wW - 200, (_wH/2) - off, 100, 100, i, { "backgroundColor" : "#efefef", 
                                                                                                  "cursor"          : "pointer" });
        _sliders[i].setAttribute("type", "range");
        _sliders[i].setAttribute("min", "1");
        _sliders[i].setAttribute("max", "100");
        off += 120;
    }

    var label = newElement("label", document.body, 50, 20, 50, 50, "", { "padding"         : "10px",
                                                                         "cursor"          : "pointer",
                                                                         "backgroundColor" : "white",
                                                                         "border"          : "solid black 3px",
                                                                         "display"         : "table" });

    _input = newElement("input", document.body, 10, 0, 50, 50, "", { "display" : "none" });
    _input.setAttribute("type", "file");
    _input.setAttribute("accept", "image/*");
    label.appendChild(document.createTextNode("Selectionnez un fichier ... "));
    label.appendChild(_input);

    _canvas = newElement("canvas", document.body, 300, 10, 800, 600, "", { "backgroundColor" : "#fafafa", 
                                                                           "border"          : " solid black" });
    _ctx = _canvas.getContext("2d");
}

function manageInput() {
    var curFiles = _input.files;
    var p = newElement("p", document.body, 50, 100, 100, 100, "", null);
    document.body.appendChild(p);
    if(curFiles.length === 0) {
        p.appendChild(document.createTextNode("Aucun fichier sélectionné pour le moment"));
    } else {
        for(var i = 0; i < curFiles.length; i++) {
            p.appendChild(document.createTextNode("- " + curFiles[i].name + ".\n" + returnFileSize(curFiles[i].size) + "."));
            var image = document.createElement("img");
            image.src = window.URL.createObjectURL(curFiles[i]);
            _images.push(image);
        }
    }
} 


function frameloop(time){
    on(_input, "change", manageInput);
    var f = 
    "saturate(" +   _sliders["saturate"].value   + "%) \
     contrast(" +   _sliders["contrast"].value   + "%) \
     brightness(" + _sliders["brightness"].value + "%)";
     
    for(var i = 0; i < _images.length; i++) {
        _ctx.filter = f;
        _ctx.drawImage(_images[i], 0, 0, _canvas.width, _canvas.height);
    }
    requestAnimationFrame(frameloop);
}