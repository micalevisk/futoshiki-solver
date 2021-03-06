var allSlots = [];
var allArrowA = [];
var allArrowB = [];
var modal;
const ordem = 4;
const qtdSetas = (ordem-1)*ordem;

const INC_CIRCULAR = (x, n) => (x+1)%n;
const LINHA = (i) => Math.ceil((i+1)/ordem);
const COLUNA = (i) => i - LINHA(i)*(ordem-1);
const VALOR = (i,j) => ordem*(i+1) - (ordem-j);

// ------------------- //
function restricoesValidasPara(i, j) {
	const v = VALOR(i,j)
	for (let indexMaior of allSlots[v].greaterThan) {
		let valMaior = allSlots[indexMaior].currentValue;
		if (valMaior && valMaior <= allSlots[v].currentValue) return false;
	}

	for (let indexMenor of allSlots[v].greaterThan) {
		let valMenor = allSlots[indexMenor].currentValue;
		if (valMenor && valMenor >= allSlots[v].currentValue) return false;
	}

	return true;
}

function linhaValidaPara(valor, i, j) {
	let colunaAtual = INC_CIRCULAR(j, ordem);

	do {
		if ( allSlots[VALOR(i, colunaAtual)].currentValue === valor ) return false;
		colunaAtual = INC_CIRCULAR(colunaAtual, ordem);
	} while (colunaAtual !== j);

	return true;
}

function colunaValidaPara(valor, i, j) {
	let linhaAtual = INC_CIRCULAR(i, ordem);

	do {
		if ( allSlots[VALOR(linhaAtual, j)].currentValue === valor ) return false;
		linhaAtual = INC_CIRCULAR(linhaAtual, ordem);
	} while (linhaAtual !== i);

	return true;
}

function regrasValidasPara(valor, i, j) {
	return restricoesValidasPara(i, j) && linhaValidaPara(valor, i, j) && colunaValidaPara(valor, i, j);
}

function validarEDefinir(valor, i, j) {
	allSlots[VALOR(i,j)].currentValue = valor;

	if ( regrasValidasPara(valor, i, j) ) return true;

	allSlots[VALOR(i, j)].currentValue = 0;
	return false;
}

function solucionar(i, j) {
	if (i === ordem) return true;
	if (j === ordem) return solucionar(i+1, 0);
	if (allSlots[VALOR(i,j)].currentValue !== 0) return solucionar(i, j+1);

	for (let cor=1; cor <= ordem; ++cor) {
		if ( validarEDefinir(cor, i, j) && solucionar(i, j+1) ) return true;
	}

	allSlots[VALOR(i, j)].currentValue = 0;
	return false;
}
// ------------------- //



function getPossibleValues(id) {
	var possibleValues = [];
	var valuesReserved = [];
	setToZero(id);

	for (let i = 0; i < ordem*ordem; i++) {
		if (allSlots[i].row == allSlots[id].row || allSlots[i].col == allSlots[id].col) {
			if (allSlots[i].currentValue !== 0) {
				valuesReserved.push(allSlots[i].currentValue);
			}
		}
	}

	for (let i = 1; i < ordem+1; i++) {
		if (valuesReserved.indexOf(i) > -1) {
			continue;
		} else {
			possibleValues.push(i);
		}
	}
	//remove 100% impossible values 5 and 1 if rules set:
	if (allSlots[id].smallerThan.length > 0) {
		if (possibleValues.indexOf(ordem) > -1) {
			possibleValues.splice(possibleValues.indexOf(ordem), 1);
		}
	}
	if (allSlots[id].greaterThan.length > 0) {
		if (possibleValues.indexOf(1) > -1) {
			possibleValues.splice(possibleValues.indexOf(1), 1);
		}
	}
	return possibleValues;
}

function setToZero(index) {
	for (let i = index; i < allSlots.length; i++) {
		if (allSlots[i].isMutable === true) {
			allSlots[i].currentValue = 0;
		}
	}
}

function Slot() {
	this.possibleValues;
	this.currentValue;
	this.greaterThan = [];
	this.smallerThan = [];
	this.row;
	this.col;
	this.isMutable;
}

function Arrow(id) {
	this.id = id;
	this.slot1_ID;
	this.slot2_ID;
	this.state = 0;
}

function createAllSlots() {
	var row = 1;
	var col = 1;
	for (let i = 0; i < ordem*ordem; i++) {
		var newSlot = new Slot();
		newSlot.row = row;
		newSlot.col = col;
		newSlot.currentValue = 0;
		newSlot.isMutable = true;
		col++;
		if (col == ordem+1) {
			row++;
			col = 1;
		}
		allSlots.push(newSlot);
	}
}

function initAllSlots() {
	var cells = document.getElementsByTagName('input');
	for (let i = 0; i < ordem*ordem; i++) {
		if ($(cells[i]).val()) {
			allSlots[i].currentValue = parseInt($(cells[i]).val(), 10);
			allSlots[i].isMutable = false;
		} else {
			allSlots[i].currentValue = 0;
			allSlots[i].isMutable = true;
		}
	}
}

function validateCells() {
  var cells = document.getElementsByTagName('input');
  for (let i = 0; i < ordem*ordem; i++) {
    if ($(cells[i]).val()) {
      let val = $(cells[i]).val();
			// let regex = /^[1-5]$/;
			let regex = RegExp(`^[1-${ordem}]$`)
      if (isNaN(parseInt(val)) || !regex.test(val)) {
        return 0;
      }
    }
  }
  return 1;
}

function createAllArrows() {
	for (let i = 1; i < qtdSetas+1; i++) {
		allArrowA.push(new Arrow(i));
		allArrowB.push(new Arrow(i));
	}
}

function initAllArrows() {
	var counter = 0;
	var offset = 0;
	for (let i = 0; i < qtdSetas; i++) {
		allArrowA[i].slot1_ID = i + offset;
		allArrowA[i].slot2_ID = i + 1 + offset;
		counter++;
		if (counter == ordem-1) {
			offset++;
			counter = 0;
		}
	}

	for (let i = 0; i < qtdSetas; i++) {
		allArrowB[i].slot1_ID = i;
		allArrowB[i].slot2_ID = i + ordem;
	}
}


function init() {//§
	createAllSlots();
	createAllArrows();
	initAllArrows();
	var arrows = document.querySelectorAll(".arrowA, .arrowB");

	for (let i = 0; i < arrows.length; i++) {
		$(arrows[i]).fadeTo("slow", 0.05);
	}
}



$('.arrowA, .arrowB').on('click', function() {
	var id = $(this).attr('id') - 1;
	var classname = $(this).attr('class');
	var selectedArrows = [];

	if (classname.includes("arrowA")) {
		selectedArrows = allArrowA;
	} else {
		selectedArrows = allArrowB;
	}
	var id1 = selectedArrows[id].slot1_ID;
	var id2 = selectedArrows[id].slot2_ID;
	var slot1 = allSlots[id1];
	var slot2 = allSlots[id2];

	if (selectedArrows[id].state === 0) {
		$(this).fadeTo("fast", 1);
		selectedArrows[id].state = 1;
		slot1.greaterThan.push(id2);
		slot2.smallerThan.push(id1);
	} else if (selectedArrows[id].state == 1) {
		selectedArrows[id].state = 2;
		slot1.greaterThan.splice(slot1.greaterThan.indexOf(id2), 1);
		slot2.smallerThan.splice(slot2.smallerThan.indexOf(id1), 1);
		slot1.smallerThan.push(id2);
		slot2.greaterThan.push(id1);
		$(this).toggleClass('rotate');
	} else if (selectedArrows[id].state == 2) {
		selectedArrows[id].state = 0;
		slot1.smallerThan.splice(slot1.smallerThan.indexOf(id2), 1);
		slot2.greaterThan.splice(slot2.greaterThan.indexOf(id1), 1);
		$(this).fadeTo("fast", 0.05);
		$(this).toggleClass('rotate');
	}
});

$('#clear').on('click', function() {
	var arrows = document.querySelectorAll(".arrowA, .arrowB");
	var slots = document.querySelectorAll("input");

	for (let i = 0; i < arrows.length; i++) {
		$(arrows[i]).fadeTo("fast", 1);
	}
	for (let i = 0; i < arrows.length; i++) {
		if ($(arrows[i]).hasClass('rotate')) {
			$(arrows[i]).removeClass('rotate');
		}
		$(arrows[i]).fadeTo("fast", 0.05);
	}
	for (let i = 0; i < arrows.length / 2; i++) {
		allArrowA[i].state = 0;
		allArrowB[i].state = 0;
	}

	for (let i = 0; i < allSlots.length; i++) {
		allSlots[i].greaterThan = [];
		allSlots[i].smallerThan = [];
		allSlots[i].currentValue = 0;
	}

	for (let i = 0; i < slots.length; i++) {
		$(slots[i]).val('');
	}
});

$('#solve').on('click', function() {
  if (validateCells() === 0) {
    displayErrorModal(2);
    return;
  }
	initAllSlots();
	var result = solucionar(0, 0);
	if (result == 1) {
		for (let i = 1; i < qtdSetas+ordem+1; i++) {
			var slot = $('input#' + i);
			slot.val(allSlots[i - 1].currentValue);
		}
	} else {
		displayErrorModal(1);
	}
});

$('#btnGuide').on('click', function() {
	modal = document.getElementById('modal-guide');
	modal.style.display = "block";
});

$('.close').on('click', function() {
	modal.style.display = "none";
});

window.onclick = function(event) {
	if (event.target == modal) {
		modal.style.display = "none";
	}
}

function displayErrorModal(errorType) {
	let errorMessage;
  if (errorType == 1) {
		errorMessage = "<p>Not possible with the current setup, adjust the rules and try again</p>";
	} else if (errorType == 2) {
    errorMessage = `<p>Use only numbers 1-${ordem}</p>`
  }
  $(".modal-body-error").empty();
  $(".modal-body-error").append(errorMessage);
  modal = document.getElementById('modal-error');
	modal.style.display = "block";
}
