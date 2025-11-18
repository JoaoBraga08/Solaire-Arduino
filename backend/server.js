const express = require("express");
const cors = require("cors");
const app = express();
const port = 3000;
 
// Último pacote recebido do ESP32
let ultimoDado = {
  deviceId: "",
  temperatura: null,
  corrente: null,
  tensao: null,
  potencia: null,
  timestamp: null,
};
 
// Middleware
app.use(cors());
app.use(express.json());
 
// ====== Rota para receber os dados ======
app.post("/sensores", (req, res) => {
  const { deviceId, temperatura, corrente, tensao, potencia } = req.body;
 
  console.log("📦 Dados recebidos do ESP32:");
  console.log(`🌡️ Temperatura: ${temperatura} °C`);
  console.log(`🔌 Corrente: ${corrente} A`);
  console.log(`⚡ Tensão: ${tensao} V`);
  console.log(`💡 Potência: ${potencia} W`);
  console.log("----------------------------------");
 
  ultimoDado = {
    deviceId,
    temperatura,
    corrente,
    tensao,
    potencia,
    timestamp: new Date().toISOString(),
  };
 
  res.status(200).send("✅ Dados recebidos com sucesso!");
});
 
// ====== Rota para consultar os dados ======
app.get("/sensores", (req, res) => {
  res.json(ultimoDado);
});
 
// ====== Inicialização ======
app.listen(port, () => {
  console.log(`🚀 Servidor rodando em http://localhost:${port}`);
});
