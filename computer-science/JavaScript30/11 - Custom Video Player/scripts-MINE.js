const player = document.querySelector('.player');
const video = player.querySelector('.viewer');
const progress = player.querySelector('.progress');
const progressBar = player.querySelector('.progress__filled');
const toggle = player.querySelector('.toggle');
const skipButtons = player.querySelectorAll('[data-skip]');
const ranges = player.querySelectorAll('.player__slider');

video.addEventListener('timeupdate', () => {
    progressBar.style.flexBasis = `${(video.currentTime / video.duration) * 100}%`
})


function toggle_play() {
    if (video.paused) {
        video.play();
        toggle.textContent = '❚ ❚';
    } else {
        video.pause();
        toggle.textContent = '►';
    }
}
video.addEventListener('click', toggle_play);
toggle.addEventListener('click', toggle_play);


let mouse_is_down = false;
function move_progress_bar(e) {
    video.currentTime = (e.offsetX / progress.offsetWidth) * video.duration;
}
progress.addEventListener('mousemove', (e) => mouse_is_down && move_progress_bar);
progress.addEventListener('click', move_progress_bar);
progress.addEventListener('mousedown', () => {mouse_is_down = true; console.log("yes")});
progress.addEventListener('mouseup', () => mouse_is_down = false);


skipButtons.forEach(button => button.addEventListener('click', () => {video.currentTime += parseFloat(button.dataset.skip)}));
ranges.forEach(range => range.addEventListener('change', () => {video[range.name] = range.value}));
