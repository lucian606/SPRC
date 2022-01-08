function createMessage(msg) {
    return {message : msg};
}

function getDate(timestamp) {
    let date = new Date(timestamp);
    return date.getFullYear() + "-" + (date.getMonth() + 1) + "-" + date.getDate();
}

module.exports = {createMessage, getDate};