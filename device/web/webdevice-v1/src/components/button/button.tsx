import React from 'react'
import {Props, StyleSheet } from './button.styled';


const Button: React.FC<Props> = ({children, ...props}) => {
  return(
    <StyleSheet {...props}>{children}</StyleSheet>
  );
}

export default Button;
