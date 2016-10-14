const $ = require('jquery')
const http = require('http')

const MAX_SPEED = 1000

let host = '10.0.1.7' // supercar.local
let speed = 0
let direction = 0 // -1 <= direction <= 1
let loading = false

function setSpeed (s, d) {
  speed = s
  speed = Math.min(MAX_SPEED, speed)
  speed = Math.max(0, speed)
  $('#speed').val(s)

  direction = Math.max(-1.0, Math.min(1.0, d))
  $('#direction').text(direction)

  if (loading) return
  loading = true

  const left = direction < 0 ? speed * (1 + direction) : speed
  const right = direction > 0 ? speed * (1 - direction) : speed

  http.get(`http://${host}/motor?l=${left}&r=${right}`, () => {
    loading = false
  })
}

$(() => {
  setSpeed(speed)

  $('body').on('keyup', e => {
    switch (e.key) {
      case 'ArrowUp':
        setSpeed(speed + 100, direction)
        break

      case 'ArrowDown':
        setSpeed(speed - 100, direction)
        break

      case 'ArrowLeft':
        setSpeed(speed, direction - 1)
        break

      case 'ArrowRight':
        setSpeed(speed, direction + 1)
        break

    }
  })

  $('#start').on('click', () => {
    setSpeed(500, 0)
  })

  $('#stop').on('click', () => {
    setSpeed(0, 0)
  })
})
