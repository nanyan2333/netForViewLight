const express = require('express');
const cors = require('cors');
const app = express();
const { exec, spawn } = require('child_process');
app.use(express.json());
app.use(cors());
app.get('/', (req, res) => {
  res.send('Hello World!');
});
app.post('/submit', (req, res) => {
  console.log(req.body);
  const child_process = spawn(req.body.file, [req.body.time]);
  var output = ''
  child_process.stdout.on('data', (data) => {
    console.log(data.toString());
    output += data.toString();
  });
  child_process.stderr.on('data', (data) => {
    console.error(`stderr: ${data}`);
  });
  child_process.on('close', (code) => {
    console.log(`child process exited with code ${code}`);
    res.send({ result: output });
  });
});

app.post('/test', (req, res) => {
  res.send({ test: 'it a test for return' })
})



app.listen(3000, () => {
  console.log('Example app listening on port 3000!');
});

