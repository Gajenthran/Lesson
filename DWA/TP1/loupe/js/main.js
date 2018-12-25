(function () {
  var requestAnimationFrame = window.requestAnimationFrame || window.mozRequestAnimationFrame || window.webkitRequestAnimationFrame || window.msRequestAnimationFrame;
  window.requestAnimationFrame = requestAnimationFrame;
})();

var canvas = document.createElement("canvas");
canvas.id = "game_canvas";     
document.body.appendChild(canvas);
var ctx = canvas.getContext("2d");

window.onload = main;

function main() {
  gameLoop();
} 
window.addEventListener("keydown", function (e) {
  keys[e.keyCode] = true;
});
window.addEventListener("keyup", function (e) {
  keys[e.keyCode] = false;
});


var avatarX = 400,
  avatarY = 300,
  velX = 0,
  velY = 0,
  keys = [],
  maxSpeed = 10;

function gameLoop() {
  whatKey();
  // var canvas = document.getElementById("canvas");
  // var ctx = canvas.getContext("2d");
  canvas.width = 800;
  canvas.height = 600;

  avatarX += velX;
  avatarY += velY;

  ctx.fillRect(avatarX, avatarY, 50, 50);
  requestAnimationFrame(gameLoop);
}

function whatKey() {
  if (keys[37]) {
    //velX = -10;
    if (velX > -maxSpeed) {
      velX -= 0.5;
    }
  }

  if (keys[39]) {
    //velX = 10;
    if (velX < maxSpeed) {
      velX += 0.5;
    }
  }
  if (keys[40]) {
    //velY = 10;
    if (velY < maxSpeed) {
      velY += 0.5;
    }
  }
  if (keys[38]) {
    //velY = -10;
    if (velY > -maxSpeed) {
      velY -= 0.5;
    }
  }
}