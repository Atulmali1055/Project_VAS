// Inside setInterval when GPT reply is ready:
const reply = await handleAssistant(transcription);
console.log("🤖 GPT Raw Reply:", reply);

try {
  // Try parsing as JSON
  const actionData = JSON.parse(reply);

  if (actionData.action === "add_todo") {
    ws.send(JSON.stringify({ type: "todo", task: actionData.task }));
    console.log("📌 Sent task to ESP32:", actionData.task);
  } else if (actionData.action === "clear_todo") {
    ws.send(JSON.stringify({ type: "clear_todo" }));
    console.log("🧹 Cleared task list on ESP32");
  }
} catch (e) {
  // Normal text reply
  ws.send(JSON.stringify({ type: "reply", text: reply }));

  // Convert reply to TTS as before
  const ttsFile = "./tmp_audio/output.wav";
  await runTTS(reply, ttsFile);
  const audioBuffer = fs.readFileSync(ttsFile);
  ws.send(audioBuffer, { binary: true });

  console.log("🔊 Sent TTS reply to ESP32");
}
