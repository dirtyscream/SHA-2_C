import jwt from 'jsonwebtoken';
import fs from 'fs';

const secret_key = 'SHA-256';

export const generate_token_user = (id) => {
  const payload = {
    user_id: id,
    role: 'user'
  }
  const options = {
    expiresIn: '2h',
  };
  const token = jwt.sign(payload, secret_key, options);
  return token;
}

export const verify_user = (request, response, next) => {
  const token = request.headers.authorization;
  if (!token) {
    return response.status(401).json({ error: 'No token' });
  }
  try {
    const decoded = jwt.verify(token, secret_key);
    if (decoded.role !== 'user') {
      return response.status(403).json({ error: "Not authorized" });
    }
    request.user = decoded;
    next();
  }
  catch (error) {
    console.error('Error:', error);
    return response.status(401).json({ error: 'error' });
  }
}

export const logging = (request, response, next) => {
  const info = `Url: ${request.url},
                Method: ${request.method}`
  fs.appendFileSync("./server.log", `${info}\n\n`)
  next()
}

