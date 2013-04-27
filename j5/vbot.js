var five = require("johnny-five"),
    board, servos, sensors = {}, ledValue = 0;;

var pins = [
  2,11,13,4,      //Knee servos' pins in the order of   Left Front, Right Front, Right Rear, Left Rear
  3,10,6,5,
  9, 12        //hip servos' pins in the order of   Left Front, Right Front, Right Rear, Left Rear
];

var standPosition = [
  160,115,120,140,      // Knee servo angles in the order of   Left Front, Right Front, Right Rear, Left Rear
  100,70,90,90,
  40, 90          // Hip servo angles  in the order of  Left Front, Right Front, Right Rear, Left Rear
];

var currentPosition = [
  160,115,120,140,
  100,70,90,90,
  40,90
];

var displacement = 30;    // Displacement angle of every step
var delayTime = 5;        // Time delay between displacement of every servo

var stretchDisplacement = 40;
var stretchDelayTime = 10;

board = new five.Board();

board.on("ready", function() {

  // Attach the servos on to the respective pins
  for (var i = 0; i < pins.length; i++) {
    five.Servo(pins[i]);
  };
  servos = new five.Servos();

  // Standup
  standUp();

  board.repl.inject({
    "moveForward": moveForward,
    "moveBackward": moveBackward,
    "moveLeft": moveLeft,
    "moveRight": moveRight,
    "standUp": standUp,
    "stretch": stretch,
    "moveServo": moveServo,
    "servos": servos
  });

  this.pinMode(36, 1);
  IREye();

  this.loop(100, function() {
    board.digitalWrite( 36, ledValue = (ledValue ? 0 : 1));
    var distance = (sensors['left'].value + sensors['right'].value + sensors['up'].value + sensors['down'].value) / 4;

    //console.log(distance);
    if (distance > 300) {
      shakeHead();
      moveBackward();
      moveForward();
      /*console.log("------------------------------------------------");
      console.log("Left: " + sensors['left'].value);
      console.log("Right: " + sensors['right'].value);
      console.log("Up: " + sensors['up'].value);
      console.log("Down: " + sensors['down'].value);
      console.log("------------------------------------------------");*/
    }
  })

  // Adding logs
  servos.each(function( servo, index ) {
    servo.on("move", function( err, degrees ) {
      //console.log("move", degrees );
    });
  });

});

function readSensor(s) {
  var sensor = s.sensor;
  s.sensorFlag = 1;
  sensor.on("read", function( err, value ) {
    //console.log(ledValue);
    if (s.sensorFlag)
      s.value = this.value;
    else
      s.value -= this.value;

    s.sensorFlag = (s.sensorFlag ? 0 : 1);
  });
}

function IREye() {
  sensors['left'] = {value: 0, sensor: new five.Sensor({
    pin: "A8",
    freq: 50
  })};

  sensors['right'] = {value: 0, sensor: new five.Sensor({
    pin: "A15",
    freq: 50
  })};

  sensors['up'] = {value: 0, sensor: new five.Sensor({
    pin: "A5",
    freq: 50
  })};

  sensors['down'] = {value: 0, sensor: new five.Sensor({
    pin: "A14",
    freq: 50
  })};

  for (var k in sensors) {
    var sensor = sensors[k].sensor;
    readSensor(sensors[k]);
  }
}

function moveServo(index, angle) {
  currentPosition[index] += angle;
  servos[index].move(currentPosition[index]);
}

// TODO: Get rid of this delay
function sleep(ms) {
  var then = new Date(), now = new Date();

  /*while (now - then < ms) {
    now = new Date();
  }*/
}

// Actions
function standUp() {
  servos.each(function(servo, index) {
    currentPosition[index] = standPosition[index];
    servo.move(standPosition[index]);
  });
}

function shakeHead() {
  for (i = 0; i < 45; i++) {
    moveServo(9, +1);
  }

  for (i = 0; i < 90; i++) {
    moveServo(9, -1);
  }  

  for (i = 0; i < 45; i++) {
    moveServo(9, +1);
  }
}

function moveForward() {
  var i = 0;

  for (i = 0; i < 45; i++) {
    moveServo(9, +1);
  }

  for(i = 0; i < displacement; i++) {     // Raise the Left Front and Right Rear legs
    moveServo(0, -1);
    moveServo(2, -1);
    sleep(delayTime); 
  }

  for(i = 0; i < displacement; i++) {      // Move the Left Front and Right Rear legs forward
    moveServo(4, +1);
    moveServo(6, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {      // Lower the Left front and Right Rear legs
    moveServo(0, +1);
    moveServo(2, +1);
    sleep(delayTime);
  }

  for (i = 0; i < 90; i++) {
    moveServo(9, -1);
  }

  for(i = 0; i < displacement; i++) {       // Move the Left Front and Right Rear legs back & Raise the Right Front and Left Rear legs
    moveServo(4, -1);
    moveServo(6, +1);
    moveServo(1, -1);
    moveServo(3, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Move the Right Front and Left Rear legs forward
    moveServo(5, -1);
    moveServo(7, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Lower the Right Front and Left Rear legs 
    moveServo(1, +1);
    moveServo(3, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Move the Right Front and Left Rear legs back
    moveServo(5, +1);
    moveServo(7, -1);
    sleep(delayTime);
  }

  for (i = 0; i < 45; i++) {
    moveServo(9, +1);
  }
}

function moveBackward() {
  var i = 0;

  for(i = 0; i < displacement; i++) {      // Raise the Left Front and Right Rear legs
    moveServo(0, -1);
    moveServo(2, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {      // Move the Left Front and Right Rear legs back
    moveServo(4, -1);
    moveServo(6, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {      // Lower the Left front and Right Rear legs
    moveServo(0, +1);
    moveServo(2, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {       // Move the Left Front and Right Rear legs forward & Raise the Right Front and Left Rear legs
    moveServo(4, +1);
    moveServo(6, -1);
    moveServo(1, -1);
    moveServo(3, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Move the Right Front and Left Rear legs back
    moveServo(5, +1);
    moveServo(7, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Lower the Right Front and Left Rear legs
    moveServo(1, +1);
    moveServo(3, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Move the Right Front and Left Rear legs forward
    moveServo(5, -1);
    moveServo(7, +1);
    sleep(delayTime);
  }
}

function moveRight() {
  var i = 0;

  for(i = 0; i < displacement; i++) {      // Raise the Left Front and Right Rear legs
    moveServo(0, -1);
    moveServo(2, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {      // Move the Left Front leg forward and Right Rear leg backward
    moveServo(4, +1);
    moveServo(6, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {      // Lower the Left front and Right Rear legs
    moveServo(0, +1);
    moveServo(2, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {       // Move the Left Front leg backward, Right Rear leg forward & Raise the Right Front and Left Rear legs
    moveServo(4, -1);
    moveServo(6, -1);
    moveServo(1, -1);
    moveServo(3, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Move the Left Rear leg forward and Right Front leg backward
    moveServo(5, +1);
    moveServo(7, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Lower the Right Front and Left Rear legs 
    moveServo(1, +1);
    moveServo(3, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Move the Left Rear leg backward and Right Front leg forward
    moveServo(5, -1);
    moveServo(7, -1);
    sleep(delayTime);
  }
}

function moveLeft() {
  var i = 0;

  for(i = 0; i < displacement; i++) {      // Raise the Left Front and Right Rear legs
    moveServo(0, -1);
    moveServo(2, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {      // Move the Left Front leg backward and Right Rear leg forward
    moveServo(4, -1);
    moveServo(6, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {      // Lower the Left front and Right Rear legs
    moveServo(0, +1);
    moveServo(2, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {       // Move the Left Front leg forward, Right Rear leg backward & Raise the Right Front and Left Rear legs
    moveServo(4, +1);
    moveServo(6, +1);
    moveServo(1, -1);
    moveServo(3, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Move the Left Rear leg backward and Right Front leg forward
    moveServo(5, -1);
    moveServo(7, -1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Lower the Right Front and Left Rear legs 
    moveServo(1, +1);
    moveServo(3, +1);
    sleep(delayTime);
  }

  for(i = 0; i < displacement; i++) {        // Move the Left Rear leg forward and Right Front leg backward
    moveServo(5, +1);
    moveServo(7, +1);
    sleep(delayTime);
  }
}

function stretch() {
  var i = 0;

  for(i = 0; i < stretchDisplacement; i++) {        // Stretch the bot towards the left            
    moveServo(0, -1);
    moveServo(1, +1);
    moveServo(2, +1);
    moveServo(3, -1);
    sleep(stretchDelayTime); 
  }
  sleep(500);

  for(i = 0; i < stretchDisplacement; i++) {        // Stretch the bot towards the right
    moveServo(0, +2);
    moveServo(1, -2);
    moveServo(2, -2);
    moveServo(3, +2);
    sleep(stretchDelayTime); 
  }
  sleep(500);

  for(i = 0; i < stretchDisplacement; i++) {        // Bring it back to initial position
    moveServo(0, -1);
    moveServo(1, +1);
    moveServo(2, +1);
    moveServo(3, -1);
    sleep(stretchDelayTime); 
  }
  sleep(500);

  for(i = 0; i < stretchDisplacement; i++) {        // Stretch the bot towards the back                                       
    moveServo(4, +1);
    moveServo(5, -1);
    moveServo(6, -1);
    moveServo(7, +1);
    sleep(stretchDelayTime); 
  }
  sleep(500);

  for(i = 0; i < stretchDisplacement; i++) {        // Stretch the bot towards the front
    moveServo(4, -2);
    moveServo(5, +2);
    moveServo(6, +2);
    moveServo(7, -2);
    sleep(stretchDelayTime); 
  }
  sleep(500);

  for(i = 0; i < stretchDisplacement; i++) {        // Bring it back to initial position
    moveServo(4, +1);
    moveServo(5, -1);
    moveServo(6, -1);
    moveServo(7, +1);
    sleep(stretchDelayTime); 
  }
  sleep(500);
}