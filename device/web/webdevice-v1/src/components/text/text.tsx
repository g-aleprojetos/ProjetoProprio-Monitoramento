import React from 'react'
import { Props, TextoStyled } from './text.styled';


const Text: React.FC<Props> = ({...props})=>{
  return<TextoStyled {...props} />;  
  
}

export default Text;