import { Sequelize, Model, DataTypes } from 'sequelize';

export const sequelize = new Sequelize({
  dialect: 'sqlite',
  storage: 'db.db'
});

export class User extends Model { }

User.init(
  {
    id: {
      type: DataTypes.INTEGER,
      primaryKey: true,
      autoIncrement: true
    },
    username: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    email: {
      type: DataTypes.STRING,
      allowNull: false,
      unique: true,
    },
    password: {
      type: DataTypes.STRING,
      allowNull: false,
    }
  },
  {
    sequelize,
    modelName: 'User',
  }
);