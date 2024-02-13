import AppIconButton from '@/components/base/AppIconButton'
import IconClose from '@/assets/icons/close.svg'
import { AppDialogProps } from '.'

export default function AppDialogTitle(props: {
  children: React.ReactNode
  onClose?: AppDialogProps['onClose']
}) {
  return (
    <div className="app-dialog-title flex justify-between p-5">
      <span className="pt-1 text-base font-bold leading-[1.5] tracking-[0.01em]">
        {props.children}
      </span>
      <AppIconButton ghost label="Close" onClick={props.onClose}>
        <IconClose />
      </AppIconButton>
    </div>
  )
}
