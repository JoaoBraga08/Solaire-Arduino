const express = require("express");

const app = express();
 
app.get("/api/dados", (req, res) => {

  const { temp, volt, corr, pot } = req.query;

  console.log(`🌡️ Temperatura: ${temp} °C | 🔋 Tensão: ${volt} V | ⚡ Corrente: ${corr} A | 🔌 Potência: ${pot} W`);

  res.send("✅ Dados recebidos com sucesso!");

});
 
app.listen(8080, "0.0.0.0", () => {

  console.log("🟢 Servidor ouvindo na porta 3001...");

});

 