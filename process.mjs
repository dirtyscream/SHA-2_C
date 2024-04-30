import { execSync } from 'child_process';
import fs from 'fs'

export const execute_command = (command) => {
  try {
    console.log(command);
    const result = execSync(command);
    const data = fs.readFileSync('hash.txt', 'utf8');
    return data;
  } catch (error) {
    console.error(`Error: ${error}`);
    return null;
  }
};

export const compare_hash = async (password, hash) => {
  try {
    const hashed_password = execute_command(`./main "${password}"`)
    if (hashed_password == hash) {
      return true;
    } else {
      return false;
    }
  } catch (error) {
    console.error(`Error: ${error}`);
  }
}