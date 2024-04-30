import express from 'express'
import * as handler from './handlers.mjs'
import * as middleware from "./middleware.mjs"

export const router = express.Router()
router.use(middleware.logging)

router.post('/api/user_register', handler.user_register)
router.post('/api/user_login', handler.user_login)
