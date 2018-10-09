class Button {
    constructor(x, y, w, h, styles) {
        this.element = newElement("div", document.body, x, y, w, h, "", styles);
    }

    changeColor() {
        let r = Math.floor((Math.random() * 255) + 1);
        let g = Math.floor((Math.random() * 255) + 1);
        let b = Math.floor((Math.random() * 255) + 1);
        this.element.style.backgroundColor = "rgb(" + r + "," + g + "," + b + ")";
    }
}

class HoverButton extends Button {
    constructor(x, y, w, h, styles) {
        super(x, y, w, h, styles);
    }

    addListeners() {
        on(this.element, "mouseover", function() {
            this.style.boxShadow = "10px 10px black"
        });

        on(this.element, "mouseout", function() {
            this.style.boxShadow = "none"
        });
    }
}


class PushButton extends Button {
    constructor(x, y, w, h, styles, shadow) {
        super(x, y, w, h, styles);
        this.shadow = shadow;
    }

    addListeners() {
        on(this.element, "mousedown", function() {
            this.style.boxShadow = "0px 0px 0px 2px inset";
        });

        on(this.element, "mouseup", function() {
            this.style.boxShadow = "none";
        });
    }
}

class HoverPushButton extends Button {
    constructor(x, y, w, h, styles, shadow) {
        super(x, y, w, h, styles);
        this.shadow = shadow;
    }

    addListeners() {
        on(this.element, "mouseover", function() {
            this.style.boxShadow = "10px 10px black"
        });

        on(this.element, "mouseout", function() {
            this.style.boxShadow = "none"
        });

        on(this.element, "mousedown", function() {
            this.style.boxShadow = "0px 0px 0px 2px inset";
        });

        on(this.element, "mouseup", function() {
            this.style.boxShadow = "none";
        });
    }
}
