import LabelMenus from '@/visualizer/components/ActionsToolbar/LabelMenus'
import AppTooltip from '@/components/base/AppTooltip'
import AppButton from '@/components/base/AppButton'

export default function ActionLables() {
  return (
    <LabelMenus
      tooltip={({ content, children }: { children: React.ReactNode; content: string }) => (
        <AppTooltip label={content} placement="bottom" interactionKind="hover-target">
          {children}
        </AppTooltip>
      )}
      button={({
        children,
        text,
        icon,
      }: {
        children: React.ReactNode
        text: string
        icon: any
      }) => (
        <AppButton text={text} icon={icon} rightIcon="caret-down">
          {children}
        </AppButton>
      )}
      selectClassName="app-select-popover"
      selectPopoverProps={{
        popoverClassName: 'app-popover app-select-popover',
      }}
    />
  )
}
