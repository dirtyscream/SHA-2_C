import express from 'express';
import * as router from './route.mjs'
import { sequelize } from './models.mjs';


const app = express();

const connect_db = async () => {
  try {
    await sequelize.authenticate()
    console.log('Connected to database.')
    await sequelize.sync()
    console.log('Models synchronized.')
  }
  catch (error) {
    console.error('Error connection to database', error)
  }
}
connect_db()


app.use(express.json())
app.use('', router.router)

app.use(function (req, res, next) {
  res.status(404).send("Not Found")
})

app.listen(3000, () => {
  console.log("Server listening on port 3000...\n")
})