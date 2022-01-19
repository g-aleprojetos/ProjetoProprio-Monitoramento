import styled from 'styled-components'
import cores from '../../resources/cores'
import fontes from '../../resources/fontes'

export interface Props{
  id?: string;
  cor?: string;
  textDecoration?: 'underline' | 'line-through';
  fontSize?: string;
  lineHeight?: string;
  marginTop?: number;
  children?: string | React.ReactNode;
  className?: string;
  peso?: 'bold' | 'normal';
  toUpper?: boolean;
};

export const TextoStyled = styled.text<Props>`
  font-size: ${(props) => props.fontSize ?? "12px"};
  font-family: ${(props) => props.peso === 'bold' ? fontes.bold : fontes.normal};
  color: ${(props) => props.cor ?? cores.black};
  margin-top: ${(props) => props.marginTop ?? 0};
  text-transform: ${(props) => (props.toUpper ? 'uppercase' : 'none')};
  text-decoration: ${(props) => props.textDecoration?? 'none'}; 
  line-height: ${(props)=> props.lineHeight ?? '150%'};
`;