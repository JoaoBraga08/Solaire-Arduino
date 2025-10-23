const express = require('express');
const cors = require('cors');
const app = express();
const port = 3001;

// Variável para armazenar o último dado recebido do ESP32
let ultimoDado = {};

// Habilita o CORS para permitir requisições de outras origens (como seu arquivo HTML)
app.use(cors());

// Habilita o Express para entender JSON no corpo das requisições
app.use(express.json());

// Rota POST para receber os dados do ESP32
// O ESP32 enviará os dados para http://SEU_IP:3000/data
app.post('/data', (req, res) => {
  console.log('Dados recebidos:', req.body);
  ultimoDado = req.body; // Armazena os dados recebidos
  res.sendStatus(200); // Responde ao ESP32 com status "OK"
});

// Rota GET para fornecer os dados ao front-end
// A página HTML buscará os dados desta URL
app.get('/data', (req, res) => {
  res.json(ultimoDado); // Envia o último dado armazenado como resposta
});

// Inicia o servidor na porta definida
app.listen(port, () => {
  console.log(`Servidor rodando em http://localhost:${port}`);
});
