import styled from 'styled-components'

export interface Props {
  id?: string;
  name?: string;
  type?: "button" | "submit" | "reset";
  disabled?: true | false;
  width?: string;
  height?: string;
  backgroundColor?: string;
  textAlign?: string;
  display?: string;
  cursor?: "pointer" | "none" | "auto";
  children?: string | React.ReactNode;
};

export const StyleSheet = styled.button<Props>`
display: inline-block;
width:${props => props.width ?? '250px'};
height: ${props => props.height ?? '40px'};
margin: 10px;
border-radius: 5px;
cursor: ${props => props.cursor ?? "default"};
border:solid 1px;


:hover{

  z-index: 2;
  -webkit-transition: all 200ms ease-in;
  -webkit-transform: scale(1.1);
  -ms-transition: all 200ms ease-in;
  -ms-transform: scale(1.1);   
  -moz-transition: all 200ms ease-in;
  -moz-transform: scale(1.1);
  transition: all 200ms ease-in;
  transform: scale(1.1); 
  box-shadow: 10px 10px 10px 5px black;
}


`;