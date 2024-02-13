import { Dialog, DialogProps, DialogBody } from '@blueprintjs/core'
import clsx from 'clsx'
import AppDialogTitle from './AppDialogTitle'

export interface AppDialogProps extends DialogProps {
  small?: boolean
  bodyClassName?: string
}

export default function AppDialog(props: AppDialogProps) {
  return (
    <Dialog
      {...props}
      isCloseButtonShown={false}
      title={
        props.title ? (
          <AppDialogTitle onClose={props.onClose}>{props.title}</AppDialogTitle>
        ) : undefined
      }
      className={clsx('app-dialog', props.className)}
    >
      <DialogBody className={clsx('app-dialog-content', props.bodyClassName)}>
        {props.children}
      </DialogBody>
    </Dialog>
  )
}
