import * as middleware from './middleware.mjs'
import * as models from './models.mjs'
import bcrypt from 'bcrypt'
import fs from 'fs'
import * as process from './process.mjs'

export const user_register = async (request, response) => {
  try {
    const { username, email, password } = request.body
    if (!username || !email || !password) {
      response.status(400).json({ error: 'Invalid data' })
      return
    }
    console.log("Not error")
    const instance_user = await models.User.findOne({ where: { email: email } })
    if (instance_user) {
      response.status(400).json({ error: "Email is used" })
      return
    }
    const hashedPassword = process.execute_command(`./main "${password}"`)
    const saved_user = await models.User.create({
      username: username,
      email: email,
      password: hashedPassword,
    })
    response.status(200).json(saved_user)
  }
  catch (error) {
    fs.appendFileSync("./server.log", `${error}\n`)
    response.status(400).json({ error: error.message || 'Internal Server Error' })
  }
}

export const user_login = async (request, response) => {
  try {
    const { username, email, password } = request.body
    if (!username || !email || !password) {
      response.status(401).json({ error: 'Invalid data' })
      return
    }
    const user = await models.User.findOne({ where: { username: username, email: email }, raw: true })
    if (!user) {
      response.status(401).json({ error: 'Invalid data' })
      return
    }
    const hashed_password = process.execute_command(`./main "${password}"`)
    if (hashed_password !== user.password) {
      response.status(401).json({ error: "Invalid data" })
      return
    }
    const token = middleware.generate_token_user(user.id)
    response.status(200).json({ token: token })
  }
  catch (error) {
    fs.appendFileSync("./server.log", `${error}\n`)
    response.status(500).json({ error: error.message || 'Internal Server Error' })
  }
}


