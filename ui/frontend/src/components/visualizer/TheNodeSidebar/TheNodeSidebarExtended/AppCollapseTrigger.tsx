import AppLinkButton from '@/components/base/AppLinkButton'
import IconPlus from '@/assets/icons/plus.svg'

export default function AppCollapseTrigger({
  children,
  onClick,
}: {
  children: React.ReactNode
  onClick: (...params: any[]) => void
}) {
  return (
    <div className="relative mt-3 flex items-center justify-center">
      <hr className="absolute top-1/2 h-[1px] w-full translate-y-[-50%] transform border-0 bg-grey-700" />
      <div className="z-[1] flex !bg-grey-900 px-4">
        <AppLinkButton rightIcon={<IconPlus />} onClick={onClick}>
          {children}
        </AppLinkButton>
      </div>
    </div>
  )
}
