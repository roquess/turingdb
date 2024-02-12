import { Button, ButtonProps } from '@blueprintjs/core'
import clsx from 'clsx'

interface Props extends ButtonProps {}

export default function AppButton(props: Props) {
  return <Button {...props} className={clsx('app-link-button', props.className)} />
}
