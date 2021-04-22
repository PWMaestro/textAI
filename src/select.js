const selectBodyNode = document.querySelector(".c-select__body");
const selectNode = document.querySelector(".c-select");

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
}

Array.from(selectBodyNode.children).forEach((prop) => {
  prop.addEventListener("click", selectOption);
});

selectNode.addEventListener("click", () => {
  selectBodyNode.classList.toggle("is-active");
});
