import { ButtonGroup, ButtonGroupProps } from '@blueprintjs/core'
import clsx from 'clsx'

interface Props extends ButtonGroupProps {}

export default function AppButtonGroup(props: Props) {
  return (
    <ButtonGroup {...props} className={clsx('app-button-group', props.className)}>
      {props.children}
    </ButtonGroup>
  )
}
