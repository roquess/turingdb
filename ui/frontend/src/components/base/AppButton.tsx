import { Button, ButtonProps } from '@blueprintjs/core'
import clsx from 'clsx'

interface Props extends ButtonProps {
  highlight?: boolean
  borderless?: boolean
}

export default function AppButton(props: Props) {
  const { highlight, className, borderless, ...rest } = props
  return (
    <Button
      {...rest}
      className={clsx(
        'app-button',
        { 'is-highlighted': highlight, 'is-borderless': borderless },
        className
      )}
    />
  )
}
