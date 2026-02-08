async function shrink() {
    const fullURL = document.getElementById("fullUrl")
    const errorMsg = document.getElementById("error-message")
    const successMsg = document.getElementById("success-message")
    const shortURLString = document.getElementById("shortUrl")
    try {
        const response = await fetch(`/shrink?url=${fullURL}`)
        if (!response.ok) {
            errorMsg.style.opacity = "100%"
            throw new Error(`Error shrinking url! Got response '${response.status}' from server.`)
        }

        const result = await response.json()
        if (!result.success) {
            errorMsg.style.opacity = "100%"
            throw new Error("Error shrinking url! Server response hasn't brought success.")
        }

        successMsg.style.opacity = "100%"
        shortURLString.textContent = result.shortUrl

    } catch (error) {
        console.error(error.message)
    }

}

function resetMsg(msgid) {
    const msg = document.getElementById(msgid)
    msg.style.opacity = "0%"
}