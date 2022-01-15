import React from 'react'
import {Props, StyleSheet } from './button.styled';


const Button: React.FC<Props> = ({children}) => {
  return(
    <StyleSheet>{children}</StyleSheet>
  );
}

export default Button;
