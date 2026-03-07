async function shrink() {
    const fullURL = document.getElementById("fullUrl")
    const errorMsg = document.getElementById("error-message")
    const successMsg = document.getElementById("success-message")
    const shortURLString = document.getElementById("shortUrl")
    try {
        const response = await fetch(`http://127.0.0.1:6944/shrink?${fullURL.value}`)
        if (!response.ok) {
            errorMsg.style.opacity = "100%"
            throw new Error(`Error shrinking url! Got response '${response.status}' from server.`)
        }

        const result = await response.json()
        if (!result.success) {
            errorMsg.style.opacity = "100%"
            throw new Error("Error shrinking url! Server response hasn't brought any success.")
        }

        successMsg.style.opacity = "100%"
        shortURLString.textContent = window.location.host + '/' + result.shortUrl
        shortURLString.href = window.location.origin + '/' + result.shortUrl

    } catch (error) {
        console.error(error.message)
    }

}

function resetMsg(msgid) {
    const msg = document.getElementById(msgid)
    msg.style.opacity = "0%"
}