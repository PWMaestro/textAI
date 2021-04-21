const selectBodyNode = document.querySelector(".c-select__body");
const selectHeaderNode = document.querySelector(".c-select__header");

function selectChildrenListener(element, listener, callback) {
  let childrenArray = Array.from(element.children);

  if (childrenArray.length === 1) {
    element.addEventListener(listener, callback);
  } else {
    childrenArray.forEach((prop) => {
      prop.addEventListener(listener, callback);
    });
  }
}

function selectOption() {
  let text = this.innerText;
  let src = this.getElementsByTagName("img")[0].getAttribute("src");
  let currentText = this.parentNode.parentNode.firstElementChild.getElementsByTagName(
    "span"
  )[0];
  let currentImg = this.parentNode.parentNode.firstElementChild.getElementsByTagName(
    "img"
  )[0];

  currentText.innerText = text;
  currentImg.src = src;

  this.parentNode.classList.toggle("is-active");
}

function selectToggle(e) {
  e.target.parentNode.parentNode.classList.toggle("is-active");
  e.target.parentNode.parentNode.lastElementChild.classList.toggle("is-active");
}

selectChildrenListener(selectBodyNode, "click", selectOption);
selectChildrenListener(selectHeaderNode, "click", selectToggle);
