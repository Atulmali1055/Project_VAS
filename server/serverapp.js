import { ChatOpenAI } from "@langchain/openai";

const llm = new ChatOpenAI({
  modelName: "gpt-4o-mini",
  temperature: 0.7,
});

// === Enhanced Assistant Handler ===
export async function handleAssistant(userInput) {
  console.log("👤 User said:", userInput);

  const systemPrompt = `
You are JARVIS, a helpful AI voice assistant.
Rules:
- Reply in a polite, witty, Jarvis-like style.
- Keep answers short (2–4 sentences).
- If the user sets a task, reminder, or alarm, return it in JSON format:
  {"action": "add_todo", "task": "Meeting at 10 AM"}
- If the user clears tasks, return:
  {"action": "clear_todo"}
- Otherwise, return normal text reply.
`;

  const response = await llm.invoke([
    { role: "system", content: systemPrompt },
    { role: "user", content: userInput }
  ]);

  return response.content;
}
