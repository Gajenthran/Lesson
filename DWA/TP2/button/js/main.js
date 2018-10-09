window.onload = main

var _wW = window.innerWidth;
var _wH = window.innerHeight;

var _buttons = {
    "normal"   : null,
    "hover"    : null,
    "push"     : null,
    "hovPush"  : null
};

var _buttonStyles = {
    "backgroundColor" : "red",
    "border"          : "none",
    "cursor"          : "pointer",
    "borderRadius"    : "5px",
    "textAlign"       : "center"
};

function main() {
    _buttons["normal"] = new Button(_wW/2 + 100, _wH/2 - 100, 100 , 100, _buttonStyles, 5);
    on(_buttons["normal"].element, "click", function() {
            _buttons["normal"].changeColor();
    });

    _buttons["hover"] = new HoverButton(_wW/2 - 100, _wH/2 - 100, 100 , 100, _buttonStyles, 5);
    _buttons["hover"].addListeners();


    _buttons["push"] = new PushButton(_wW/2 - 300, _wH/2 - 100, 100 , 100, _buttonStyles, 5);
    _buttons["push"].addListeners();

    _buttons["hovPush"] = new HoverPushButton(_wW/2 - 100, _wH/2 + 100, 100 , 100, _buttonStyles, 5);
    _buttons["hovPush"].addListeners();
}   
