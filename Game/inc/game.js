window.onload = function () {

  var inimsg = "Presione una tecla para comenzar"

  var deseado = document.getElementById("deseado");
  var desc = document.getElementById("desc");
  var decoded = document.getElementById("decoded");
  var bien = document.getElementById("bien");
  var mal = document.getElementById("mal");
  var bienp = document.getElementById("bienp");
  var malp = document.getElementById("malp");
  var total = document.getElementById("total");
  var tiempo = document.getElementById("tiempo");
  var ctiempo = document.getElementById("ctiempo");
  var startBtn = document.getElementById("start");
  var config = document.getElementById("config");

  var timer1;
  var timer;
  var stopped = true;
  var clear = true;
  var list = [];

  var tone = new AudioContext()

  function get_symbol() {
    let res = [];
    if (document.getElementById("letras").checked) res = res.concat(symbols.letras);
    if (document.getElementById("numeros").checked) res = res.concat(symbols.numeros);
    if (document.getElementById("especiales").checked) res = res.concat(symbols.especiales);
    if (document.getElementById("proc").checked) res = res.concat(symbols.proc);
    if (document.getElementById("abbr").checked) res = res.concat(symbols.abbr);
    if (document.getElementById("qcode").checked) res = res.concat(symbols.qcode);
    if (document.getElementById("palabras").checked) res = res.concat(symbols.palabras);
    if (document.getElementById("callsigns").checked) res = res.concat(symbols.callsigns);
    return res;
  };

  function word_dice() {
    let rnd = Math.round(Math.random() * (list.length-1));

    if (list[rnd].length > 3) {
      let size = (100/(list[rnd].length*1.3));
      deseado.style.fontSize = size+'vh';
    } else {
      deseado.style.fontSize = '';
    }

    deseado.innerHTML = list[rnd].normalize("NFD").replace(/[\u0300-\u036f]/g, "");

    if (symbols.desc[list[rnd]]) {
      desc.innerHTML = symbols.desc[list[rnd]];
    } else {
      desc.innerHTML = "";
    };
  }

  function correcto() {
    decoded.classList.add('green');
    decoded.classList.remove('red');
    bien.innerHTML = parseInt(bien.innerHTML)+1;
    calc_tot();
    genTone("sine", 300);
  }

  function incorrecto() {
    decoded.classList.add('red');
    decoded.classList.remove('green');
    mal.innerHTML = parseInt(mal.innerHTML)+1;
    calc_tot();
    genTone("sawtooth", 150);
  }

  function calc_tot() {
    total.innerHTML = (parseInt(bien.innerHTML)*5)-parseInt(mal.innerHTML);  
    bienp.innerHTML = Math.floor(parseInt(bien.innerHTML)/(parseInt(bien.innerHTML)+parseInt(mal.innerHTML))*100)+'%'; 
    malp.innerHTML = Math.ceil(parseInt(mal.innerHTML)/(parseInt(bien.innerHTML)+parseInt(mal.innerHTML))*100)+'%';
  }

  function genTone(type, freq){
    var o = tone.createOscillator()
    var g = tone.createGain()

    o.connect(g)
    g.connect(tone.destination)

    o.type = type
    o.frequency.value = freq;
    o.start(0)
    g.gain.exponentialRampToValueAtTime(
      0.0001, tone.currentTime + 1.5
    )
  }

  function start() {
    reset_score();
    stopped = false;
    if (ctiempo.value != 0) {
      timer1 = setInterval(function(){
        if (timer <= 0) {
          stop();
        } else {
          timer = parseInt(timer)-1
          update_timer(timer);
        }
      }, 1000);
    } else {
      timer1 = setInterval(function(){
        timer = parseInt(timer)+1
        update_timer(timer);
      }, 1000);
    }
  }

  function reset_score() {
    bien.innerHTML = "0";
    mal.innerHTML = "0";
    bienp.innerHTML = "0%";
    malp.innerHTML = "0%";
    total.innerHTML = "0";
  }

  function stop() {
    startBtn.classList.add("green")
    startBtn.classList.remove("red")

    startBtn.innerHTML = 'START';

    decoded.classList.remove("green");
    decoded.classList.remove("red");

    config.style.display = "block";
    stopped = true;
    clearInterval(timer1);
    decoded.innerHTML = '';
    desc.innerHTML = '';
    deseado.innerHTML = '';
  }

  function update_timer(timer) {
    tiempo.innerHTML = new Date(timer * 1000).toISOString().substring(14, 19);
  }

  function pre_start() {
    list = get_symbol();
    if (list.length == 0) {
      alert("Debe seleccionar al menos un grupo de caracteres")
      stop();
      return;
    }

    timer = ctiempo.value;
    update_timer(timer);

    startBtn.classList.add("red")
    startBtn.classList.remove("green")
    startBtn.innerHTML = 'STOP';
    desc.innerHTML = inimsg;

    stopped = false;
    config.style.display = "none";
    lose_focus();
  }

  function lose_focus() {
    let el = document.querySelector( ':focus' );
    if( el ) el.blur();
  }

  startBtn.onclick = function(){
    if (stopped) {
      pre_start();
    } else {
      stop();
    }
  };

  document.addEventListener("keypress", function onPress(event) {
    if (stopped) {
      if (event.key == "Enter") {
        pre_start()
      }
      return;
    }
    if (deseado.innerHTML == "") {
      start();
      word_dice();
      return;
    }

    if (clear) {
      if (event.key == ' ') return;
      decoded.innerHTML = '';
      clear = false;
    }
    if (decoded.innerHTML == '' && event.key == ' ') return;

    decoded.innerHTML = decoded.innerHTML.concat(event.key.toUpperCase());

    if (deseado.innerHTML.startsWith(decoded.innerHTML)) {
      if (decoded.innerHTML.length >= deseado.innerHTML.length) {
        if (deseado.innerHTML == decoded.innerHTML) {
          correcto();
          word_dice();
        } else {
          incorrecto();
        }
        clear = true;
      }
    } else {
      incorrecto();
      clear = true;
    }
  });
};
